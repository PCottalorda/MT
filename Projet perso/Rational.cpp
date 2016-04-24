#include "Rational.h"

/*

Rational Rational::zero(0, 1);

Rational Rational::one(1, 1);

Rational::Rational(int64_t num, uint64_t den) : num(num), den(den) {
	if (den == 0)
		throw std::exception("Attempt to declare a Rational with a null denominator");

	simplify();
};

Rational::Rational(const Rational &r) {
	num = r.num;
	den = r.den;
};

Rational::~Rational()
{
}

uint64_t Rational::pgcd(uint64_t m, uint64_t n) {
	uint64_t tmp;

	if (m == 0 || n == 0) {
		return 1;
	}

	while (n != 0) {
		tmp = m;
		m = n;
		n = tmp % n;
	}

	return m;
}

uint64_t Rational::lcm(uint64_t m, uint64_t n) {
	uint64_t a = m / pgcd(m, n);
	return n*a;
}

void Rational::simplify() {
	// TODO: Remove!!
	//std::cerr << "Rational::simplify called [" << this << "]" << std::endl;
	if (num == 0) {
		den = 1;
	}
	else {
		uint64_t __num = abs(num);
		uint64_t gcd = pgcd(__num, den);
		num = num / (int64_t)gcd;
		den = den / gcd;
	}
}

Rational& Rational::operator=(const Rational& r) {
	num = r.num;
	den = r.den;

	return *this;
};

Rational& Rational::operator+=(const Rational& r) {
	uint64_t res_den = den * r.den;
	int64_t num1 = num * r.den;
	int64_t num2 = r.num * den;
	num = num1 + num2;
	den = res_den;
	simplify();
	return *this;
};

Rational& Rational::operator-=(const Rational& r) {
	uint64_t res_den = den * r.den;
	int64_t num1 = num * r.den;
	int64_t num2 = r.num * den;
	num = num1 - num2;
	den = res_den;
	simplify();
	return *this;
};

Rational& Rational::operator*=(const Rational& r) {
	num *= r.num;
	den *= r.den;
	simplify();
	return *this;
};

Rational& Rational::operator/=(const Rational& r) {
	if (r.den == 0) {
		throw std::exception("Division by a zero in the class Rational"); // TODO: Modify with a clearer exception
	}
	else {
		num *= r.den;
		den *= r.num;

		simplify();
	}
	return *this;
}

bool Rational::isNull() {
	return num == 0;
}


Rational operator+ (const Rational &r1, const Rational &r2) {
	Rational r(r1);
	return r += r2;
}

Rational operator- (const Rational &r1, const Rational &r2) {
	Rational r(r1);
	return r -= r2;
}

Rational operator* (const Rational &r1, const Rational &r2) {
	Rational r(r1);
	return r *= r2;
};

Rational operator/ (const Rational &r1, const Rational &r2) {
	Rational r(r1);
	return r /= r2;
};


bool Rational::operator==(const Rational& r) {
	return num * ((int64_t)r.den) == ((int64_t)den) * r.num;
};

bool Rational::operator!=(const Rational& r) {
	return num * ((int64_t)r.den) != ((int64_t)den) * r.num;
};

bool Rational::operator<=(const Rational& r) {
	return num * ((int64_t)r.den) <= ((int64_t)den) * r.num;
};

bool Rational::operator>=(const Rational& r) {
	return num * ((int64_t)r.den) >= ((int64_t)den) * r.num;
};

bool Rational::operator<(const Rational& r) {
	return num * ((int64_t)r.den) < ((int64_t)den) * r.num;
};

bool Rational::operator>(const Rational& r) {
	return num * ((int64_t)r.den) > ((int64_t)den) * r.num;
};

Rational Rational::middle(const Rational &r1, const Rational& r2) {
	Rational r = r1 + r2;
	r.den *= 2;
	r.simplify();

	return r;
};

float Rational::floatApproximate() const {
	float __res = 0.0f;
	float coeff = 1.0f;
	uint64_t __num = (uint64_t)abs(num);
	uint64_t __den = den;
	for (int i = 0; i < 64; i++) {
		uint64_t div = __num / __den;
		__num = __num % __den;
		__res += coeff*div;
		__num *= 10;
		coeff /= 10.0f;
	}

	return num < 0.0f ? -__res : __res;		// Return the approximation with the sign
};

Rational abs(const Rational& r) {
	return Rational(abs(r.num), r.den);
}

std::ostream& operator<<(std::ostream& os, const Rational& obj)
{
	// write obj to stream
	os << obj.num << "/" << obj.den;
	return os;
}

*/