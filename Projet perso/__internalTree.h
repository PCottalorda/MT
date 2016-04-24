#pragma once

#include <vector>
#include <functional>

#include "Rational.h"
#include "Rational2DPoint.h"
#include "Rational2DForm.h"



class __internalTree {

public:
	Rational2DPoint x1;
	Rational2DPoint x2;

	bool __isCone;
	bool __isFlat;

	Rational2DPoint ConeIntersec;

	Rational2DForm FlatNorm;		// Has sense only is isFlat
	Rational2DForm ConeNorm1;		// Has sense only if ifCone
	Rational2DForm ConeNorm2;		// Has sense only if ifCone

	std::function<Rational(const Rational2DPoint&)> norm;

	__internalTree *child_1 = nullptr;
	__internalTree *child_2 = nullptr;


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
