#pragma once
#include "Rational2DPoint.h"



class Segment {

public:

	class DegeneratedSegment : std::exception{};
	using IntersectSol = struct {
		bool exists = false;
		bool isUnique = false;
		Rational2DPoint inter;

		bool solutionIsUnique() const {
			return exists && isUnique;
		}
	};

	Segment(const Rational2DPoint& p1, const Rational2DPoint& p2);
	Segment(const Segment& other);
	Segment(Segment&& other);
	Segment& operator=(const Segment& other);
	Segment& operator=(Segment&& other);
	~Segment();

	bool isAlignedWith(const Rational2DPoint& X) const;
	bool segmentContains(const Rational2DPoint& X) const;
	bool isParallelTo(const Segment& S) const;
	Rational2DPoint directorVector() const;
	Rational2DPoint intersectionWith(const Segment& S) const;
	bool hasAsExtremity(const Rational2DPoint& X) const;
	IntersectSol uniqueIntersectionWith(const Segment& S) const;
	Rational lambdaCoeff(const Rational2DPoint& X) const;

	Rational2DPoint p1;
	Rational2DPoint p2;
};



class IntersectionManager
{
	friend class Segment;

public:
	IntersectionManager();
	~IntersectionManager();

	const Rational2DPoint* requestPoint(const Rational2DPoint& p);

private:
	std::vector<Rational2DPoint> intersectionPointsSet;
};


