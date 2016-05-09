#pragma once
#include "Rational2DPoint.h"
#include <iterator>



class Segment {

public:

	friend class SplitSegmentWrapper;

	class DegeneratedSegment : std::exception{};
	using IntersectSol = struct {
		bool exists = false;
		bool isUnique = false;
		Rational2DPoint inter;

		bool solutionIsUnique() const {
			return exists && isUnique;
		}
	};

	Segment(const Rational2DPoint& p1 = Rational2DPoint(), const Rational2DPoint& p2 = Rational2DPoint());
	Segment(const Segment& other);
	Segment(Segment&& other);
	Segment& operator=(const Segment& other);
	Segment& operator=(Segment&& other);
	~Segment();


	friend bool operator==(const Segment& lhs, const Segment& rhs) {
		return lhs.p1 == rhs.p1
			&& lhs.p2 == rhs.p2;
	}

	friend bool operator!=(const Segment& lhs, const Segment& rhs) {
		return !(lhs == rhs);
	}

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

class SplitSegmentWrapper {

public:
	SplitSegmentWrapper(Segment& base,SplitSegmentWrapper* father = nullptr);
	SplitSegmentWrapper(const SplitSegmentWrapper& seg);
	~SplitSegmentWrapper();

	friend bool operator==(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs);
	friend bool operator!=(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs);

	class IncorrectSplit : std::exception {};
	class SplitSegmentIteratorOutOfRange : std::exception {};
	class iterator : public std::iterator < std::input_iterator_tag, SplitSegmentWrapper > {
		friend class SplitSegmentWrapper;

	public:
		iterator(const iterator& it);
		iterator& operator++();
		iterator operator++(int);
		bool operator==(const iterator& rhs) const;
		bool operator!=(const iterator& rhs) const;
		SplitSegmentWrapper& operator*() const;

	private:
		explicit iterator(SplitSegmentWrapper* base);
		SplitSegmentWrapper* base;
	};

	iterator begin();
	iterator end();



	void split(const Rational2DPoint& splitPoint);
	bool isSplit() const;
	bool isRoot() const;

private:	// Methods

	void __checkValidity() const;

private:	// Members
	bool __isSplit;
	Segment base;
	SplitSegmentWrapper* father;
	SplitSegmentWrapper* son1;
	SplitSegmentWrapper* son2;
};



class PolyLineCurve {
	
	friend class iterator;

public:

	class PolyLineCurveOutOfRangeIteratorException : std::exception {};
	class iterator : public std::iterator<std::input_iterator_tag, SplitSegmentWrapper> {
		friend class PolyLineCurve;
	public:
		iterator(const iterator& it);
		~iterator();
		iterator& operator++();
		iterator operator++(int);
		friend bool operator==(const iterator& lhs, const iterator& rhs);
		friend bool operator!=(const iterator& lhs, const iterator& rhs);
		SplitSegmentWrapper& operator*() const;;

	private:
		explicit iterator(std::vector<SplitSegmentWrapper>& curve);
		void pointEnd();

	private:	// Members
		std::vector<SplitSegmentWrapper>& curve;
		std::vector<SplitSegmentWrapper>::iterator internalIt;
		SplitSegmentWrapper::iterator* subInternalIt;
	};

	explicit PolyLineCurve(std::vector<SplitSegmentWrapper> segs);

	iterator begin();
	iterator end();


private:
	std::vector<SplitSegmentWrapper> segments;
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


