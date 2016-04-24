#pragma once

#include <cstdint>
#include <iostream>
#include <exception>

#include <boost\multiprecision\cpp_int.hpp>

using Rational = boost::multiprecision::cpp_rational;

/*
class Rational
{
// Public methods
public:
	Rational(int64_t num = 0, uint64_t den = 1);
	Rational(const Rational &r);
	~Rational();

	Rational& operator=(const Rational& r);

	Rational& operator+=(const Rational& r);
	Rational& operator-=(const Rational& r);
	Rational& operator*=(const Rational& r);
	Rational& operator/=(const Rational& r);

	friend Rational operator+ (const Rational &r1, const Rational &r2);
	friend Rational operator- (const Rational &r1, const Rational &r2);
	friend Rational operator* (const Rational &r1, const Rational &r2);
	friend Rational operator/ (const Rational &r1, const Rational &r2);

	friend Rational abs(const Rational& r);

	bool operator==(const Rational& r);
	bool operator!=(const Rational& r);
	bool operator<=(const Rational& r);
	bool operator>=(const Rational& r);
	bool operator>(const Rational& r);
	bool operator<(const Rational& r);

	bool isNull();

	float floatApproximate() const;

	friend std::ostream& operator<<(std::ostream& os, const Rational& obj);

// Public static methods
public:
	static Rational middle(const Rational &r1, const Rational& r2);

// Public static elements
public:
	static Rational zero;
	static Rational one;


// Private members
private:
	int64_t num;
	uint64_t den;

// Private methods
private:
	void simplify();

// Private static methods
private:
	static uint64_t pgcd(uint64_t m, uint64_t n);
	static uint64_t lcm(uint64_t m, uint64_t n);
};

*/