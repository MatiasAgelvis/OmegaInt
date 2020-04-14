#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <type_traits>
#include <sstream>

#pragma once

#define _ std::cin.get();

typedef unsigned long long u64;

// Maximum number of digits a field can have. (System depedent)
// const unsigned MAXDIGITS = floor(std::numeric_limits<u64>::digits * log10(2) / 2);
const unsigned MAXDIGITS = pow(10, floor(log10(std::numeric_limits<u64>::digits)));
// const unsigned MAXDIGITS = 5; // for testing purposes

// Maximum value that a field can have. (System depedent), values are strictly less than
const u64 MAXFIELDVALUE = pow(10,MAXDIGITS);
// Value al which it is safe to multiply two fields of an OmegaInt
const u64 ALLOWED = sqrt(MAXFIELDVALUE -1);
// #define MAXFIELDVALUE (pow(10,MAXDIGITS) - 1);

using std::cout;
using std::endl;
using std::string;

class OmegaInt
{
	private:
		// Number of pieces the number is split into
		u64 TOTALFIELDS;
		// Pieces that comprice the number
		u64* NUMBERS = NULL;
		// Sing, is it positive?
		bool isPOSITIVE;
		// Copy function
		void _copy(OmegaInt const & other);
		// Addition function
		OmegaInt _add(OmegaInt const & other) const;
		// Subtraction function
		OmegaInt _subtract(OmegaInt const & other) const;
	
		// Maintenance routine
		void _maintenance();
		void _reSize(u64 newSize);

	public:
		// Construction and Deletion
			// Empty
			OmegaInt();

			// From a number represented in a string
			OmegaInt(std::string num);
			OmegaInt(char const* num);

			// Number of fields and sing setted all to zero
			OmegaInt(u64 fields, bool pos);
			// Number of fields and sing setted all to a value
			OmegaInt(u64 fields, u64* nums, bool pos);
			// Copy constructor
			OmegaInt(OmegaInt const & other);
			// Destructor
			~OmegaInt();


		// Getters
			// Returns the number of fields in the OmegaInt
			unsigned fields() const;
			// Returns the sing of the OmegaInt
			bool sing() const;
			// Absolute value, returns a positive copy of the object
			OmegaInt abs() const;
			// Returns the number of digits
			u64 digits() const;

		// Setters
			// Changes the sing of the OmegaInt
			void changeSing();
			// set field 'i' to a value
			void set (const unsigned i, u64 value);
			// Assingment Operator
				OmegaInt const & operator = (OmegaInt const & other);
				OmegaInt const & operator = (std::string num);
				OmegaInt const & operator = (char const* num);


		// Getter
			u64 operator [] (const unsigned i) const;

		// Comparison Operators
			bool operator == (OmegaInt const &other) const;
			bool operator != (OmegaInt const &other) const;
			bool operator >  (OmegaInt const &other) const;
			bool operator >= (OmegaInt const &other) const;
			bool operator <= (OmegaInt const &other) const;
			bool operator <  (OmegaInt const &other) const;

		// Arithmetic Operators
			OmegaInt operator +  (OmegaInt const & other) const;
			OmegaInt operator -  (OmegaInt const & other) const;
			OmegaInt operator *  (OmegaInt const & other) const;
			OmegaInt operator /  (OmegaInt const & other) const;
			void     operator += (OmegaInt const & other);
			void     operator -= (OmegaInt const & other);
			void     operator ++ ();
			void     operator -- ();


		// Output Methods
			std::string toString() const;
			void debugPrint();
			void print();
			friend std::ostream& operator<<(std::ostream & os, const OmegaInt & A);

		// Templated Methods
			template < typename T> OmegaInt(T foo);
			template < typename T> OmegaInt const & operator = (T num);
			template < typename T > bool operator == (T const num) const;
			template < typename T > bool operator >  (T num) const;
			template < typename T > OmegaInt operator + (T num) const;
			template < typename T > OmegaInt operator - (T num) const;
			template < typename T > OmegaInt operator * (T num) const;
			template < typename T > OmegaInt operator / (T num) const;
			template < typename T > void operator += (T num);
			template < typename T > void operator -= (T num);

// Multiplicating by a power of 10
	OmegaInt _e10(u64 power) const;
// split the OmegaInt from a certain digit
	OmegaInt _split_from(u64 split) const;
// split the OmegaInt to a certain digit
	OmegaInt _split_to(u64 split) const;
	OmegaInt _karatsuba(OmegaInt const & other) const;

};

// Templated Methods
template < typename T>
OmegaInt::OmegaInt(T foo)
{
	try
	{
		if(! std::is_arithmetic<T>::value){throw 1;}
	}
	catch(int e)
	{ cout << "Incorrect constructor, for pointer types the correct constructor is OmegaInt(u64 fields, u64* nums, bool pos);"; }
	isPOSITIVE = foo >= 0;
	u64 num = std::is_unsigned<T>::value? foo : /*std::is_integral<T>::value? std::llabs(foo) :*/ std::fabs(foo);

	TOTALFIELDS = num != 0? floor( (log10(num) )/ MAXDIGITS ) + 1 : 1;

	NUMBERS = (u64*) calloc( TOTALFIELDS, sizeof(u64) );
	for (unsigned i = 0; i < TOTALFIELDS; ++i)
	{
		NUMBERS[i] = num % MAXFIELDVALUE;
		num = (num - (num % MAXFIELDVALUE)) / MAXFIELDVALUE;
	}
};

template < typename T>
OmegaInt const & OmegaInt::operator = (T num)
{
	// If it is not a numerical type return an empty OmegaInt
	if (!std::is_integral<T>::value and !std::is_floating_point<T>::value)
		{ puts ("Error: Assinging non-numeric type to an OmegaInt"); return * new OmegaInt(); }

	_copy(OmegaInt(num));
	return *this;
};

template < typename T >
bool OmegaInt::operator == (T const num) const { return *this == (OmegaInt(num)); };

template < typename T >
bool OmegaInt::operator >  (T num) const { return *this > (OmegaInt(num)); }

template < typename T >
OmegaInt OmegaInt::operator + (T num) const { return *this + (OmegaInt(num)); }

template < typename T >
OmegaInt OmegaInt::operator - (T num) const { return *this - (OmegaInt(num)); }

template < typename T >
OmegaInt OmegaInt::operator * (T num) const { return *this * (OmegaInt(num)); }

template < typename T >
OmegaInt OmegaInt::operator / (T num) const { return *this / (OmegaInt(num)); }

template < typename T >
void OmegaInt::operator += (T num) { *this += (OmegaInt(num)); }

template < typename T >
void OmegaInt::operator -= (T num) { *this -= (OmegaInt(num)); }