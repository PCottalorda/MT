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
	Rational2DPoint red1 = p1 - p2;
	Rational2DPoint red2 = S.p2 - S.p1;

	Rational determinant = Rational2DPoint::det(red1, red2);
	assert(determinant != 0);

	Rational2DPoint calcInt = S.p2 - p2;
	Rational lambda1 = (red2.y * calcInt.x - red2.x * calcInt.y) / determinant;
	//=== Compute only for vérification purpose ===
	Rational lambda2 = (-red1.y * calcInt.x + red1.x * calcInt.y) / determinant;
	Rational2DPoint verif_res = lambda2 * S.p1 + (1 - lambda2) * S.p2;
	//=============================================
	Rational2DPoint result = lambda1 * p1 + (1 - lambda1) * p2;

	assert(result == verif_res);
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

/*
SplitSegmentWrapper::iterator::iterator(SplitSegmentWrapper* base): base(base) {
	if (base != nullptr) {
		assert(!base->isSplit());
	}
}

SplitSegmentWrapper::iterator::iterator(const iterator& it): base(it.base) {
}

SplitSegmentWrapper::iterator& SplitSegmentWrapper::iterator::operator++() {
	SplitSegmentWrapper* current = base;
	if (current == nullptr) {
		throw SplitSegmentIteratorOutOfRange();
	}
	SplitSegmentWrapper* next = nullptr;
	while (!current->isRoot()) {
		// We check from what side we come
		if (current == current->father->son1) { // current is the first son of his father
			next = current->father->son2;
			// We descend on the youngest leaf
			while (next->isSplit()) {
				next = next->son1;
			}
			break; // The next has been found
		} else { // current is the second son of his father! 
			current = current->father;
		}
	}

	base = next;
	return *this;
}

SplitSegmentWrapper::iterator SplitSegmentWrapper::iterator::operator++(int) {
	iterator tmp(*this);
	operator++();
	return tmp;
}

bool SplitSegmentWrapper::iterator::operator==(const iterator& rhs) const {
	return base == rhs.base;
}

bool SplitSegmentWrapper::iterator::operator!=(const iterator& rhs) const {
	return !operator==(rhs);
}

SplitSegmentWrapper& SplitSegmentWrapper::iterator::operator*() const {
	return *base;
}

SplitSegmentWrapper::iterator SplitSegmentWrapper::begin() {
	// We compute the first son
	SplitSegmentWrapper* current = this;
	while (current->isSplit()) {
		current = current->son1;
	}
	return iterator(current);
}

SplitSegmentWrapper::iterator SplitSegmentWrapper::end() {
	SplitSegmentWrapper* current = this;
	return iterator(nullptr);
}
*/

SplitSegmentWrapper::SplitSegmentWrapper(const Segment& base, SplitSegmentWrapper* father):
	base(base),
	__isSplit(false),
	son1(nullptr),
	son2(nullptr),
	father(father) {

}

SplitSegmentWrapper::SplitSegmentWrapper(const SplitSegmentWrapper& seg):
	__isSplit(seg.isSplit()),
	base(seg.base),
	father(nullptr) {
	if (__isSplit) {
		son1 = new SplitSegmentWrapper(*seg.son1);
		son1->father = this;
		son2 = new SplitSegmentWrapper(*seg.son2);
		son2->father = this;
	} else {
		son1 = nullptr;
		son2 = nullptr;
	}
}

SplitSegmentWrapper::~SplitSegmentWrapper() {
	delete son1;
	delete son2;
}

size_t SplitSegmentWrapper::size() const {
	return isSplit() ? son1->size() + son2->size() : 1;
}

Segment::IntersectSol SplitSegmentWrapper::intersectionWith(const SplitSegmentWrapper& s) const {
	return base.uniqueIntersectionWith(s.base);
}

void SplitSegmentWrapper::split(const Rational2DPoint& splitPoint) {
	if (!base.segmentContains(splitPoint)) {
		throw IncorrectSplit();
	}

	if (isSplit()) {
		if (son1->base.segmentContains(splitPoint)) {
			son1->split(splitPoint);
		} else if (son2->base.segmentContains(splitPoint)) {
			son2->split(splitPoint);
		} else {
			assert(false);
		}
	} else {
		if (splitPoint == base.p1 || splitPoint == base.p2) {
			// Nothing to split!
		} else {
			// The split is coherent in this case
			Segment b(base.p1, splitPoint);
			son1 = new SplitSegmentWrapper(b, this);
			b = Segment(splitPoint, base.p2);
			son1 = new SplitSegmentWrapper(b, this);
			__isSplit = true;
		}
	}
}

bool SplitSegmentWrapper::isSplit() const {
	return __isSplit;
}

bool SplitSegmentWrapper::isRoot() const {
	return father == nullptr;
}

void SplitSegmentWrapper::__checkValidity() const {
	if (isSplit()) {
		assert(son1 != nullptr);
		assert(son2 != nullptr);
		son1->__checkValidity();
		son2->__checkValidity();
		assert(son1->base.p1 == base.p1);
		assert(son2->base.p2 == base.p2);
		assert(son1->base.p2 == son2->base.p1);
		assert(base.segmentContains(son1->base.p2));
	} else {
		assert(son1 == nullptr);
		assert(son2 == nullptr);
	}
}

PolyLineCurve::iterator::iterator(const iterator& it):
	curve(it.curve),
	internalIt(it.internalIt) {
	/*
	if (it.subInternalIt != nullptr) {
		subInternalIt = new SplitSegmentWrapper::iterator(*(it.subInternalIt));
	} else {
		subInternalIt = nullptr;
	}
	*/
}

PolyLineCurve::iterator& PolyLineCurve::iterator::operator=(const iterator& it) {
	curve = it.curve;
	internalIt = it.internalIt;
	/*
	if (it.subInternalIt != nullptr) {
		delete subInternalIt;
		subInternalIt = new SplitSegmentWrapper::iterator(*(it.subInternalIt));
	} else {
		subInternalIt = nullptr;
	}
	*/
	return *this;
}

PolyLineCurve::iterator::~iterator() {
	//delete subInternalIt;
}

PolyLineCurve::iterator& PolyLineCurve::iterator::operator++() {
	if (internalIt == curve.end()) {
		throw PolyLineCurveOutOfRangeIteratorException();
	}
	++internalIt;
	/*
	assert(subInternalIt != nullptr);
	if (*subInternalIt == curve.back().end()) {
		throw PolyLineCurveOutOfRangeIteratorException();
	}
	
	++(*subInternalIt);
	if (*subInternalIt == internalIt->end()) {
		++internalIt;
		if (internalIt != curve.end()) {
			*subInternalIt = internalIt->begin();
		} else {
			delete subInternalIt;
			subInternalIt = nullptr;
		}
	}
	*/
	return *this;
}

PolyLineCurve::iterator PolyLineCurve::iterator::operator++(int) {
	iterator tmp(*this);
	operator++();
	return tmp;
}

SplitSegmentWrapper& PolyLineCurve::iterator::operator*() const {
	/*
	if (subInternalIt == nullptr) {
		throw PolyLineCurveOutOfRangeIteratorException();
	} else {
		try {
			return *(*subInternalIt);
		} catch (std::exception&) {
			throw PolyLineCurveOutOfRangeIteratorException();
		}
	}
	*/
	return *internalIt;
}

PolyLineCurve::iterator::iterator(std::vector<SplitSegmentWrapper>& curve):
	curve(curve),
	internalIt(curve.begin()) {
	//subInternalIt(curve.empty() ? nullptr : new SplitSegmentWrapper::iterator(curve.begin()->begin())) {
}

/*
void PolyLineCurve::iterator::pointEnd() {
	if (curve.empty()) {
		internalIt = curve.end();
		subInternalIt = nullptr;
	} else {
		internalIt = curve.end();
		subInternalIt = nullptr;
	}
}
*/

PolyLineCurve::PolyLineCurve(std::vector<SplitSegmentWrapper> segs): segments(segs) {
}

PolyLineCurve::PolyLineCurve(const std::vector<Segment>& segs) {
	for (const Segment& s : segs) {
		SplitSegmentWrapper sSW(s);
		segments.push_back(SplitSegmentWrapper(s));
	}
}

PolyLineCurve::iterator PolyLineCurve::begin() {
	return iterator(segments);
}

PolyLineCurve::iterator PolyLineCurve::end() {
	iterator it(begin());
	it.internalIt = it.curve.end();
	//it.pointEnd();
	return it;
}

size_t PolyLineCurve::size() const {
	size_t res = 0;
	for (const SplitSegmentWrapper& s : segments) {
		res += s.size();
	}
	return res;
}

IntersectionManager::IntersectionManager() {
}


IntersectionManager::~IntersectionManager() {
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

bool operator==(const Segment& lhs, const Segment& rhs) {
	return lhs.p1 == rhs.p1
		&& lhs.p2 == rhs.p2;
}

bool operator!=(const Segment& lhs, const Segment& rhs) {
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Segment& s) {
	os << "[" << s.p1 << " | " << s.p2 << "]";
	return os;
}

bool operator==(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs) {
	return lhs.__isSplit == rhs.__isSplit
	       && lhs.base == rhs.base
	       && lhs.isSplit() ? *lhs.son1 == *rhs.son1 && *lhs.son2 == *rhs.son2 : true;
}

bool operator!=(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs) {
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const SplitSegmentWrapper& sW) {
	os << sW.base;
	return os;
}

bool operator==(const PolyLineCurve::iterator& lhs, const PolyLineCurve::iterator& rhs) {
	return lhs.internalIt == rhs.internalIt;
	/*
	bool pre_res = true;
	if (lhs.subInternalIt == nullptr) {
		if (rhs.subInternalIt == nullptr) {
			pre_res = true;
		} else {
			return false;
		}
	} else if (rhs.subInternalIt == nullptr) {
		return false;
	} else {
		pre_res =  *lhs.subInternalIt == *rhs.subInternalIt;
	}
	return lhs.curve == rhs.curve
		&& lhs.internalIt == rhs.internalIt && pre_res;
	*/
}

bool operator!=(const PolyLineCurve::iterator& lhs, const PolyLineCurve::iterator& rhs) {
	return !(lhs == rhs);
}
