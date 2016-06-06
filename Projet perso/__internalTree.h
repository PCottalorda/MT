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
#include <functional>

#include "Rational2DPoint.h"


class __internalTree {

public:
	Rational2DPoint x1;
	Rational2DPoint x2;

	bool __isCone;
	bool __isFlat;

	Rational2DPoint ConeIntersec;

	Rational2DForm FlatNorm; // Has sense only is isFlat
	Rational2DForm ConeNorm1; // Has sense only if ifCone
	Rational2DForm ConeNorm2; // Has sense only if ifCone

	std::function<Rational(const Rational2DPoint&)> norm;

	__internalTree* child_1 = nullptr;
	__internalTree* child_2 = nullptr;


public:
	__internalTree(Rational2DPoint __x1, Rational2DPoint __x2, std::function<Rational(const Rational2DPoint&)> norm);
	~__internalTree();

	void normalize(Rational2DPoint& x);

	Rational2DPoint normed(const Rational2DPoint& x);

	bool isFlat();
	bool isCone();

	bool isLeaf();
	bool isFlatSegment(const Rational2DPoint& x1, const Rational2DPoint& x2, std::function<Rational(const Rational2DPoint&)> norm);

	void prettyPrint();

	void __internal_computeUnitaryBall();

	void collectData(std::vector<Rational2DForm>& forms, std::vector<Rational2DPoint>& v);
};
