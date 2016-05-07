#include "IntersectionManager.h"


Segment::Segment(const Rational2DPoint& p1, const Rational2DPoint& p2): p1(p1),
                                                                        p2(p2) {
	if (p1 == p2) throw DegeneratedSegment();
}

Segment::~Segment() {
}

bool Segment::isAlignedWith(const Rational2DPoint& X) const {
	return Rational2DPoint::det(p2 - p1, X - p1) == 0;
}

bool Segment::segmentContains(const Rational2DPoint& X) const {
	if (!isAlignedWith(X)) {
		return false;
	} else {
		Rational lambda = lambdaCoeff(X);
		return lambda <= 1 && lambda >= 0;
	}
}

bool Segment::isParallelTo(const Segment& S) const {
	return Rational2DPoint::det(directorVector(), S.directorVector()) == 0;
}

Rational2DPoint Segment::directorVector() const {
	return p2 - p1;
}

Rational2DPoint Segment::intersectionWith(const Segment& S) const {
	assert(!isParallelTo(S));
	Rational2DPoint red1 = -1 * directorVector();
	Rational2DPoint red2 = S.directorVector();

	Rational determinant = Rational2DPoint::det(red1, red2);
	assert(determinant != 0);

	Rational2DPoint calcInt = S.p2 - p2;
	Rational lambda1 = (red1.x * calcInt.x - red1.y * calcInt.y) / determinant;

	Rational2DPoint result = lambda1 * p1 + (1 - lambda1) * p2;
	assert(isAlignedWith(result));
	assert(S.isAlignedWith(result));

	return result;
}

Segment::Segment(const Segment& other): p1(other.p1),
                                        p2(other.p2) {
}

Segment::Segment(Segment&& other): p1(std::move(other.p1)),
                                   p2(std::move(other.p2)) {
}

Segment& Segment::operator=(const Segment& other) {
	if (this == &other)
		return *this;
	p1 = other.p1;
	p2 = other.p2;
	return *this;
}

Segment& Segment::operator=(Segment&& other) {
	if (this == &other)
		return *this;
	p1 = std::move(other.p1);
	p2 = std::move(other.p2);
	return *this;
}

bool Segment::hasAsExtremity(const Rational2DPoint& X) const {
	return X == p1 || X == p2;
}

Segment::IntersectSol Segment::uniqueIntersectionWith(const Segment& S) const {
	IntersectSol solution;
	if (isParallelTo(S)) {
		if (S.isAlignedWith(p1)) {
			// Does it intersect?
			if (S.segmentContains(p1) || S.segmentContains(p2) || segmentContains(S.p1) || segmentContains(S.p2)) {
				// There is an intersection!
				solution.exists = true;
				try {
					if (p1 == S.p1) {
						Segment newSeg(p2, S.p2);
						assert(newSeg.isAlignedWith(p1));
						if (newSeg.segmentContains(p1)) {
							solution.exists = true;
							solution.isUnique = true;
							solution.inter = p1;
						} else {
							// Infinite solution
							solution.exists = true;
						}
					} else if (p2 == S.p1) {
						Segment newSeg(p1, S.p2);
						assert(newSeg.isAlignedWith(p2));
						if (newSeg.segmentContains(p2)) {
							solution.exists = true;
							solution.isUnique = true;
							solution.inter = p2;
						} else {
							// Infinite solution
							solution.exists = true;
						}
					} else if (p1 == S.p2) {
						Segment newSeg(p2, S.p1);
						assert(newSeg.isAlignedWith(p1));
						if (newSeg.segmentContains(p1)) {
							solution.exists = true;
							solution.isUnique = true;
							solution.inter = p1;
						} else {
							// Infinite solution
							solution.exists = true;
						}
					} else if (p2 == S.p2) {
						Segment newSeg(p1, S.p1);
						assert(newSeg.isAlignedWith(p2));
						if (newSeg.segmentContains(p2)) {
							solution.exists = true;
							solution.isUnique = true;
							solution.inter = p2;
						} else {
							// Infinite solution
							solution.exists = true;
						}
					} else {
						// Infinite solution;
					}
				} catch (DegeneratedSegment) {
					// Only happens when infinite intersection occurs!
					solution.exists = true;
				}
			} else {
				// No solution
			}
		} else {
			// No solution
		}
	} else {
		Rational2DPoint inter = intersectionWith(S);
		if (segmentContains(inter) && S.segmentContains(inter)) {
			solution.exists = true;
			solution.isUnique = true;
			solution.inter = inter;
		} else {
			// No solution
		}
	}

	return solution;
}

Rational Segment::lambdaCoeff(const Rational2DPoint& X) const {
	assert(isAlignedWith(X));
	Rational2DPoint baseSeg = p2 - p1;
	Rational2DPoint compSeg = X - p1;
	if (baseSeg.x != 0) {
		return compSeg.x / baseSeg.x;
	} else {
		assert(baseSeg.y != 0);
		return compSeg.y / baseSeg.y;
	}
}

IntersectionManager::IntersectionManager()
{
}


IntersectionManager::~IntersectionManager()
{
}

const Rational2DPoint* IntersectionManager::requestPoint(const Rational2DPoint& p) {
	auto request = std::find(intersectionPointsSet.begin(), intersectionPointsSet.end(), p);
	if (request == intersectionPointsSet.end()) { // the request hasn't been found...
		// In this case, we create a new point
		intersectionPointsSet.push_back(p);
		return &(intersectionPointsSet.back());
	} else { // the request has been found...
		// We return the adresse of the point found
		return &(*request);
	}
}
