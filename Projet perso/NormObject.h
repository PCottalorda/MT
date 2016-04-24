#pragma once

#include <vector>

#include <ostream>
#include <random>

#include "Rational.h"
#include "Rational2DPoint.h"

class NormObject {

	using NormObjectException = std::exception;

	// Public methods
public:
	NormObject();
	NormObject(std::string& str);
	virtual ~NormObject();

	float operator()(float x, float y) const;
	Rational operator()(const Rational& x, const Rational& y) const;
	Rational operator()(const Rational2DPoint& p) const;

	// Private intern class
private:
	class RationalSlopeRep final {

		friend class NormObject;

	public:
		RationalSlopeRep();
		RationalSlopeRep(int a, int b);
		~RationalSlopeRep();

		bool isProportional(const RationalSlopeRep& slopeRep) const;

		friend std::ostream& operator<<(std::ostream& os, const RationalSlopeRep& slopeRep);

	public:
		int a;
		int b;

		void randomize();

	private:
		static std::default_random_engine generator;
		static std::uniform_int_distribution<int> distribution;
		static std::function<int()> RNG;

	};

	friend std::ostream& operator<<(std::ostream& os, const RationalSlopeRep& slopeRep);
	friend std::ostream& operator<<(std::ostream& os, const NormObject& slopeRep);

	// Private static methods
	static bool isDigit(const char c);
	static int getDigit(const char c);

	// Private members
	std::vector<RationalSlopeRep> v;


};
