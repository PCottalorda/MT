#include "NormObject.h"


#include <chrono>
#include <cstdint>

std::default_random_engine NormObject::RationalSlopeRep::generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> NormObject::RationalSlopeRep::distribution(-100, 100);
std::function<int()> NormObject::RationalSlopeRep::RNG(std::bind(distribution, generator));

void NormObject::RationalSlopeRep::randomize() {
	do {
		a = RNG();
		b = RNG();
	} while (a == 0 && b == 0);
}

NormObject::NormObject() {
	int nb_set;

	do {
		nb_set = abs(RationalSlopeRep::RNG()) % 20;
	} while (nb_set <= 1);

	auto check = [](const std::vector<RationalSlopeRep> &v) {
		const RationalSlopeRep& refSlope = v[0];
		for (unsigned int i = 1; i < v.size(); i++) {
			if (!(refSlope.isProportional(v[i])))
				return true;
		}

		return false;
	};

	do {
		v.clear();

		for (int i = 0; i < nb_set; i++){
			v.push_back(RationalSlopeRep());		// Random slope rep
		}
	} while (!check(v));
}

std::ostream& operator<< (std::ostream& os, const NormObject::RationalSlopeRep& slopeRep) {
	os << "[" << slopeRep.a << "," << slopeRep.b << "]";
	return os;
}

NormObject::RationalSlopeRep::RationalSlopeRep(int a, int b) :
a(a),
b(b)
{
	if ((a == 0) && (b == 0)) {
		throw NormObject::NormObjectException();
	}
}

NormObject::RationalSlopeRep::RationalSlopeRep() {
	randomize();
}

NormObject::RationalSlopeRep::~RationalSlopeRep() {}

bool NormObject::RationalSlopeRep::isProportional(const NormObject::RationalSlopeRep &slopeRep) const {
	int64_t __a = slopeRep.a;
	int64_t __b = slopeRep.b;
	return (a*__b + b*__a) == 0;
}


NormObject::NormObject(std::string& str) {

	enum __AUTOM_STATE
	{
		START,
		GEN_OPEN,
		GEN_CLOSE,
		LOC_OPEN,
		LOC_CLOSE,
		READ_NUMBER_1,
		READ_NUMBER_2,
		COMMA_LOC,
		COMMA_GEN,
		MINUS_1,
		MINUS_2
	};


	int64_t number_read_1 = 0;
	int64_t number_read_2 = 0;
	bool negative_read = false;

	__AUTOM_STATE state = START;

	if (str.size() == 0) {
		throw NormObject::NormObjectException();
	}



	for (size_t index = 0; index < str.size(); index++) {

		char curr = str[index];

		switch (state)
		{
		case START:
			if (curr == '[') {
				state = GEN_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case GEN_OPEN:
			if (curr == '[') {
				state = LOC_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case GEN_CLOSE:
			break;
		case LOC_OPEN:
			number_read_1 = 0;
			number_read_2 = 0;
			if (curr == '-') {
				negative_read = true;
				state = MINUS_1;
			}
			else if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case LOC_CLOSE:
			if (curr == ',') {
				state = COMMA_GEN;
			}
			else if (curr == ']') {
				state = GEN_CLOSE;
			}
			else {
				throw NormObject::NormObjectException();
			}
			negative_read = false;
			v.push_back(RationalSlopeRep((int)number_read_1, (int)number_read_2));
			break;
		case READ_NUMBER_1:
			if (curr == ',') {
				if (negative_read) {
					number_read_1 *= -1;
				}
				negative_read = false;
				state = COMMA_LOC;
			}
			else if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case MINUS_1:
			if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case READ_NUMBER_2:
			if (curr == ']') {
				if (negative_read) {
					number_read_2 *= -1;
				}
				negative_read = false;
				state = LOC_CLOSE;
			}
			else if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case MINUS_2:
			if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case COMMA_LOC:
			if (curr == '-') {
				negative_read = true;
				state = MINUS_2;
			}
			else if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case COMMA_GEN:
			if (curr == '[') {
				state = LOC_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		default:
			// Unreachable code
			throw NormObject::NormObjectException();
		}

	}

	if (v.size() < 2) {
		throw NormObject::NormObjectException();
	}

	bool isNorm = false;

	RationalSlopeRep& slopeRepRef = v[0];

	for (size_t i = 1; i < v.size(); i++) {
		if (!slopeRepRef.isProportional(v[i])) {
			isNorm = true;
			break;
		}
	}

	if (!isNorm) {
		throw NormObject::NormObjectException();
	}
}

NormObject::~NormObject() {

}

float NormObject::operator() (float x, float y) const {
	float res = 0;
	for (size_t i = 0; i < v.size(); i++) {
		float a = (float)(v[i].a);
		float b = (float)(v[i].b);
		res += abs(b*x - a*y);
	}
	return res;
}

Rational NormObject::operator() (const Rational& x, const Rational& y) const{
	Rational res(0, 1);
	for (size_t i = 0; i < v.size(); i++) {
		int __a = v[i].a;
		int __b = v[i].b;
		Rational a(__a, 1);
		Rational b(__b, 1);
		res += abs(b*x - a*y);
	}
	return res;
}

Rational NormObject::operator() (const Rational2DPoint& p) const {
	return operator()(p.x, p.y);
}

bool NormObject::isDigit(const char c) {
	return (c - '0') >= 0 && (c - '0') <= 9;
}

int NormObject::getDigit(const char c) {
	return c - '0';
}

std::ostream& operator<< (std::ostream& os, const NormObject& nO) {
	os << "[";
	for (size_t i = 0; i < (nO.v.size() - 1); i++) {
		os << nO.v[i] << ",";
	}
	os << nO.v.back() << "]";
	return os;
}