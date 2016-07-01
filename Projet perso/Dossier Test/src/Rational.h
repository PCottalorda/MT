#pragma once


/*****************************************************************************
*                                                                            *
*  Copyright 2016 Paul Cottalorda                                            *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
******************************************************************************/


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