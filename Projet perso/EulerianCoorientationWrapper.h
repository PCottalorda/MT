#pragma once
#include "HomologieValue.h"
#include <set>
#include <algorithm>
#include <cassert>
#include <functional>
#include "Rational.h"
#include "Rational2DPoint.h"

class EulerianCoorientationWrapper
{
public:

	EulerianCoorientationWrapper(const HomologieValue& hom = HomologieValue(2), bool on_boundiary = false);
	EulerianCoorientationWrapper(const EulerianCoorientationWrapper& other);
	EulerianCoorientationWrapper(EulerianCoorientationWrapper&& other);

	EulerianCoorientationWrapper& operator=(const EulerianCoorientationWrapper& other);
	EulerianCoorientationWrapper& operator=(EulerianCoorientationWrapper&& other);

	~EulerianCoorientationWrapper();


	// TODO: Pas optimal!
	static std::vector<EulerianCoorientationWrapper> ConvexHullMarking2D(const std::set<HomologieValue>& s);
	static std::vector<Rational2DPoint> computeConvexHull2D(const std::set<Rational2DPoint>& s);
	static std::vector<Rational2DPoint> computeConvexHull2D(const std::vector<Rational2DPoint>& v);
	static std::vector<Rational2DPoint> planeCorrespondance(const HomologieValue& A, const HomologieValue& B, int i, int j);
	static long long int cross(const HomologieValue& O, const HomologieValue& A, const HomologieValue& B);
	static Rational cross(const Rational2DPoint& O, const Rational2DPoint& A, const Rational2DPoint& B);

	// using Pick theorem 
	static Rational volume(const std::vector<EulerianCoorientationWrapper>& v);
	static Rational symplecticArea(const std::set<HomologieValue>& s);

	static Rational area(const Rational2DPoint& A, const Rational2DPoint& B, const Rational2DPoint& C);
	static Rational computeArea(const std::vector<Rational2DPoint>& v);

private:
	HomologieValue hom;
	bool onBoundiary;
};

