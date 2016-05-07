#include "Rational2DPoint.h"

Rational2DForm compute_rat_form(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	Rational2DForm f(p2.y - p1.y, p1.x - p2.x);
	Rational r = p1.x * p2.y - p2.x * p1.y;
	f /= r; // Exception will be send if r is null;
	return f;
}

Rational2DPoint::Rational2DPoint(const Rational& x, const Rational& y) : x(x), y(y) {
};

Rational2DPoint::~Rational2DPoint() {
};

Rational2DPoint& Rational2DPoint::operator=(const Rational2DPoint& p) {
	x = p.x;
	y = p.y;
	return *this;
};

Rational2DPoint& Rational2DPoint::operator+=(const Rational2DPoint& p) {
	x += p.x;
	y += p.y;
	return *this;
};

Rational2DPoint& Rational2DPoint::operator-=(const Rational2DPoint& p) {
	x -= p.x;
	y -= p.y;
	return *this;
};

Rational2DPoint& Rational2DPoint::operator*=(const Rational& r) {
	x *= r;
	y *= r;
	return *this;
};

Rational2DPoint& Rational2DPoint::operator/=(const Rational& r) {
	x /= r;
	y /= r;
	return *this;
};

Rational2DPoint operator+(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	Rational2DPoint p(p1);
	p += p2;
	return p;
};

Rational2DPoint operator-(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	Rational2DPoint p(p1);
	p -= p2;
	return p;
};

Rational2DPoint operator*(const Rational& r, const Rational2DPoint& p) {
	Rational2DPoint pp(p);
	pp *= r;
	return pp;
};

Rational2DPoint operator*(const Rational2DPoint& p, const Rational& r) {
	Rational2DPoint pp(p);
	pp *= r;
	return pp;
};

Rational2DPoint operator/(const Rational2DPoint& p, const Rational& r) {
	Rational2DPoint pp(p);
	pp /= r;
	return pp;
}

bool operator==(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	return (p1.x == p2.x) && (p1.y == p2.y);
}

bool operator!=(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	return !(p1 == p2);
};


bool Rational2DPoint::operator==(const Rational2DPoint& p) {
	return (x == p.x) && (y == p.y);
};

bool Rational2DPoint::operator!=(const Rational2DPoint& p) {
	return (x != p.x) || (y != p.y);
};

Rational2DPoint Rational2DPoint::middle(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	return Rational(1, 2) * (p1 + p2);
};


Rational Rational2DPoint::dotProduct(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	return p1.x * p2.x + p1.y * p2.y;
};

Rational Rational2DPoint::det(const Rational2DPoint& p1, const Rational2DPoint& p2) {
	return p1.x * p2.y - p1.y * p2.x;
};

sf::Vector2f Rational2DPoint::toSFMLVector2f() {
	return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
};

std::ostream& operator<<(std::ostream& os, const Rational2DPoint& obj) {
	os << "( " << obj.x << " ; " << obj.y << " )";
	return os;
};
