#pragma once

#include "Rational.h"
#include "Rational2DForm.h"
#include <SFML/System/Vector2.hpp>

class Rational2DPoint
{
public:
	Rational2DPoint(const Rational& x = Rational(0,1), const Rational& y = Rational(0,1));
	~Rational2DPoint();

	Rational2DPoint& operator=(const Rational2DPoint& p);

	Rational2DPoint& operator+=(const Rational2DPoint& p);
	Rational2DPoint& operator-=(const Rational2DPoint& p);
	Rational2DPoint& operator*=(const Rational& r);
	Rational2DPoint& operator/=(const Rational& r);

	friend Rational2DPoint operator+ (const Rational2DPoint &p1, const Rational2DPoint &p2);
	friend Rational2DPoint operator- (const Rational2DPoint &p1, const Rational2DPoint &p2);
	friend Rational2DPoint operator* (const Rational &r, const Rational2DPoint &p);
	friend Rational2DPoint operator* (const Rational2DPoint &p, const Rational &r);
	friend Rational2DPoint operator/ (const Rational2DPoint &p, const Rational &r);

	bool operator==(const Rational2DPoint &p);
	bool operator!=(const Rational2DPoint &p);

	friend Rational2DPoint middle(const Rational2DPoint &p1, const Rational2DPoint &p2);
	friend Rational dotProduct(const Rational2DPoint &p1, const Rational2DPoint &p2);
	friend Rational det(const Rational2DPoint &p1, const Rational2DPoint &p2);
	friend std::ostream& operator<<(std::ostream& os, const Rational2DPoint& obj);

	sf::Vector2f toSFMLVector2f();

	friend Rational2DForm compute_rat_form(const Rational2DPoint& p1, const Rational2DPoint& p2);

public:
	Rational x;
	Rational y;
};

