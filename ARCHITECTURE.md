# OmegaInt — Architecture Notes

Cheat sheet for talking about this project.

## What It Is

A from-scratch arbitrary-precision integer library in C++14. Stores arbitrarily large integers as arrays of 64-bit unsigned fields and supports arithmetic, comparisons, and mixed-type operations with `int`, `std::string`, etc.

## Storage Model

- Each `OmegaInt` holds a `u64*` array called `NUMBERS` and a field count `TOTALFIELDS`.
- Each field stores up to **18 decimal digits** (base 10^18).
- `MAXFIELDVALUE = 10^18`, derived from `floor(log10(ULLONG_MAX) - 1)`.
- Sign is stored as a separate `bool isPOSITIVE`.
- The number `123456789012345678901234567890` becomes two fields: `[1234567890123456789, 12345]`.

Why base 10^18 instead of 2^64? Makes `toString()` trivial — just `sprintf` each field with zero-padding. Tradeoff is ~1 bit wasted per field.

## Key Algorithms

### Addition & Subtraction

Digit-by-digit from least significant field, with carry/borrow propagation. `operator+` dispatches to `_add()` or `_subtract()` based on the signs of both operands (4 sign combinations handled).

### Multiplication — Karatsuba

`operator*` calls `_karatsuba()`, a recursive divide-and-conquer algorithm:

1. Split each number A and B in half at midpoint `m2`.
2. Compute `A_low * B_low` (z0), `A_high * B_high` (z2), and `(A_low + A_high) * (B_low + B_high)` (z1).
3. Result = `z2 * 10^(2*m2) + (z1 - z2 - z0) * 10^m2 + z0`.

This reduces 4 sub-multiplications to 3, giving O(n^1.585) vs naive O(n^2).

### Division — Long Division

`operator/` and `operator%` call `_longDiv()`, which converts to strings, strips common trailing zeros, then performs digit-by-digit long division. The division helper `Division::divide()` works on decimal strings directly.

### Power

`operator^` is simple iterative exponentiation (repeated squaring not implemented — just loops `result *= base` for `exp` times).

### Maintenance

`_maintenance()` is called after operations to normalize the number: handles carry propagation across fields, strips leading zero fields, and handles the zero case.

## API Design

### Mixed-Type Operations

Templated constructors and operators allow seamless interop:

```cpp
OmegaInt("9") ^ 3 == "729"   // string + int
OmegaInt(27) - "7"           // int + string
-12 + OmegaInt(4)            // friend operator: int + OmegaInt
```

The pattern: each templated operator wraps the argument in `OmegaInt(T)` then delegates to the `OmegaInt` operator.

### Naming

- `sing()` — returns the sign (true = positive). Should probably be `sign()`.
- `changeSing()` — flips the sign. Should probably be `changeSign()`.
- `fields()` — number of 18-digit fields.
- `TOTALFIELDS` — internal field count.
- `MAXDIGITS` — 18, the max digits per field.

## File Structure

```
OmegaInt.hpp      — Class declaration + template implementations (~257 lines)
OmegaInt.cpp      — All method implementations (~717 lines)
OmegaUtils.hpp    — GCD, LCM, swap, min, max (template-heavy)
OmegaUtils.cpp    — swap, min, max implementations
test.cpp          — 77 test cases covering all operations
SingleFile/       — Self-contained single-header version (concatenated)
```

## Test Suite (test.cpp)

Run with `./test all` or individual suites: `+`, `-`, `x`, `/`, `utils`, `bool`.

Covers:
- All arithmetic with positive, negative, and mixed signs
- Large number multiplication (up to 212 digits)
- Division edge cases (tail zeros, single-word denominators)
- GCD and LCM
- Even/odd checks on very large numbers
- All comparison operators
- Template constructors and friend operators

## Known Limitations

- Raw `calloc`/`free` memory management (no RAII, no move semantics)
- Power operator is O(n) multiplications, not O(log n)
- No bit operations (removed during development)
- The single-header file is a concatenation, not a properly structured header
