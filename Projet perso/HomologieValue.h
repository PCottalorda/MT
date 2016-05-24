#pragma once

#include<vector>

class HomologieValue
{
public:

	class DimensionException : public std::exception {};

	HomologieValue(unsigned int dimension);;
	HomologieValue(const HomologieValue& other);
	HomologieValue(HomologieValue&& other);
	~HomologieValue();

	HomologieValue& operator=(const HomologieValue& other);
	HomologieValue& operator=(HomologieValue&& other);

	friend bool operator==(const HomologieValue& lhs, const HomologieValue& rhs);
	friend bool operator!=(const HomologieValue& lhs, const HomologieValue& rhs);

	int operator[](int i) const;
	int& operator[](int i);

	HomologieValue operator-() const;

	HomologieValue& operator+=(const HomologieValue& rhs);
	HomologieValue& operator-=(const HomologieValue& rhs);
	friend HomologieValue operator+(HomologieValue lhs, const HomologieValue& rhs);
	friend HomologieValue operator-(HomologieValue lhs, const HomologieValue& rhs);
	unsigned int dimension() const;

	friend std::ostream& operator<<(std::ostream& os, const HomologieValue& hVal) {
		os << "[";
		for (unsigned int i = 0; i < hVal.base.size() - 1; ++i) {
			os << hVal.base[i]  << ",";
		}
		if (hVal.dimension() != 0) {
			os << hVal.base.back();
		}
		os << "]";
		return os;
	}

private:
	unsigned int dim;
	std::vector<int> base;
};

