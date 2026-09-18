# OmegaInt

[![CodeFactor](https://www.codefactor.io/repository/github/matiasagelvis/omegaint/badge)](https://www.codefactor.io/repository/github/matiasagelvis/omegaint)

Arbitrarily big integers in C++14.

OmegaInt stores numbers as an array of 64-bit fields, each holding 18 decimal digits. It trades a bit of memory efficiency for a convenient API — you can mix `OmegaInt` with `int`, `std::string`, and other `OmegaInt` values seamlessly:

```cpp
OmegaInt(27) - "7"          // 20
OmegaInt(15) + int(14)      // 29
OmegaInt(9) ^ 3 == "729"    // true
```

## Features

- **Arithmetic:** `+`, `-`, `*`, `/`, `%`, `^` (power)
- **Comparisons:** `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Mixed types:** operate on `OmegaInt` with `int`, `long long`, `std::string`, etc.
- **Karatsuba multiplication** for efficient large-number multiplication
- **GCD, LCM, min, max** via `OmegaUtils`

## Building

```bash
make          # compile
./test all    # run all tests
./test help   # see available test suites
```

## Single Header

A self-contained single-header version is available at `SingleFile/OmegaInt.hpp` — just include it and you're good to go.

## Acknowledgments

Build system from [UIUC's CS courses](https://github.com/illinois-cs241) by Wade Fagen-Ulmschneider, Jeffrey Tolar, and Eric Huber.

![logo](logo.png)
