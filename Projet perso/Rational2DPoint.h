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


#include "Rational.h"
#include "Rational2DForm.h"
#include <SFML/System/Vector2.hpp>

class Rational2DPoint {
public:
	Rational2DPoint(const Rational& x = Rational(0, 1), const Rational& y = Rational(0, 1));
	~Rational2DPoint();

	Rational2DPoint& operator=(const Rational2DPoint& p);

	Rational2DPoint& operator+=(const Rational2DPoint& p);
	Rational2DPoint& operator-=(const Rational2DPoint& p);
	Rational2DPoint& operator*=(const Rational& r);
	Rational2DPoint& operator/=(const Rational& r);

	Rational2DPoint operator-() const;

	friend Rational2DPoint operator+(const Rational2DPoint& p1, const Rational2DPoint& p2);
	friend Rational2DPoint operator-(const Rational2DPoint& p1, const Rational2DPoint& p2);
	friend Rational2DPoint operator*(const Rational& r, const Rational2DPoint& p);
	friend Rational2DPoint operator*(const Rational2DPoint& p, const Rational& r);
	friend Rational2DPoint operator/(const Rational2DPoint& p, const Rational& r);

	friend bool operator==(const Rational2DPoint& p1, const Rational2DPoint& p2);
	friend bool operator!=(const Rational2DPoint& p1, const Rational2DPoint& p2);


	bool operator==(const Rational2DPoint& p);
	bool operator!=(const Rational2DPoint& p);

	static Rational2DPoint middle(const Rational2DPoint& p1, const Rational2DPoint& p2);
	static Rational dotProduct(const Rational2DPoint& p1, const Rational2DPoint& p2);
	static Rational det(const Rational2DPoint& p1, const Rational2DPoint& p2);

	friend std::ostream& operator<<(std::ostream& os, const Rational2DPoint& obj);

	sf::Vector2f toSFMLVector2f() const;

	friend Rational2DForm compute_rat_form(const Rational2DPoint& p1, const Rational2DPoint& p2);


	void prettyPrint() const {
		std::cout << "(" << static_cast<float>(x) << "," << static_cast<float>(y) << ")" << std::endl;
	}
public:
	Rational x;
	Rational y;
};
