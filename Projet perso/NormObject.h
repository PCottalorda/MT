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
