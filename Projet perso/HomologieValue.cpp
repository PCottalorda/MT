#include "HomologieValue.h"

#include <ostream>

HomologieValue::HomologieValue(unsigned dimension):
	dim(dimension),
	base(dimension) {
	for (unsigned int i = 0; i < dimension; ++i)
		base[i] = 0;
}

HomologieValue::~HomologieValue() {
}

HomologieValue::HomologieValue(const HomologieValue& other): dim(other.dim),
                                                             base(other.base) {
}

HomologieValue::HomologieValue(HomologieValue&& other): dim(other.dim),
                                                        base(std::move(other.base)) {
}

HomologieValue& HomologieValue::operator=(const HomologieValue& other) {
	if (this == &other)
		return *this;
	dim = other.dim;
	base = other.base;
	return *this;
}

HomologieValue& HomologieValue::operator=(HomologieValue&& other) {
	if (this == &other)
		return *this;
	dim = other.dim;
	base = std::move(other.base);
	return *this;
}

bool operator==(const HomologieValue& lhs, const HomologieValue& rhs) {
	return lhs.dim == rhs.dim
		&& lhs.base == rhs.base;
}

bool operator!=(const HomologieValue& lhs, const HomologieValue& rhs) {
	return !(lhs == rhs);
}

int HomologieValue::operator[](int i) const {
	return base[i];
}

int& HomologieValue::operator[](int i) {
	return base[i];
}

HomologieValue HomologieValue::operator-() const {
	HomologieValue res(*this);
	for (unsigned int i = 0; i < dimension(); ++i) {
		res[i] = -res[i];
	}
	return res;
}

HomologieValue& HomologieValue::operator+=(const HomologieValue& rhs) {
	if (dimension() != rhs.dimension()) {
		throw DimensionException();
	}
	for (unsigned int i = 0; i < dim; ++i) {
		base[i] += rhs[i];
	}
	return *this;
}

HomologieValue& HomologieValue::operator-=(const HomologieValue& rhs) {
	if (dimension() != rhs.dimension()) {
		throw DimensionException();
	}
	for (unsigned int i = 0; i < dim; ++i) {
		base[i] -= rhs[i];
	}
	return *this;
}

HomologieValue operator+(HomologieValue lhs, const HomologieValue& rhs) {
	lhs += rhs;
	return lhs;
}

HomologieValue operator-(HomologieValue lhs, const HomologieValue& rhs) {
	lhs -= rhs;
	return lhs;
}

std::ostream& operator<<(std::ostream& os, const HomologieValue& hVal) {
	os << "[";
	for (unsigned int i = 0; i < hVal.base.size() - 1; ++i) {
		os << hVal.base[i] << ",";
	}
	if (hVal.dimension() != 0) {
		os << hVal.base.back();
	}
	os << "]";
	return os;
}

unsigned HomologieValue::dimension() const {
	return dim;
}
