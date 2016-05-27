#pragma once
#include "Rational2DPoint.h"
#include "InternalPositionSystem.h"
#include "Graph.h"

#include <iterator>
#include "SettingWindow.h"

class Segment {

public:

	friend class SplitSegmentWrapper;
	friend class IntersectionManager;

	class DegeneratedSegment : std::exception {
	};

	class IntersectSol {
	public:
		IntersectSol();

		bool exists = false;
		bool isUnique = false;
		RationalPoint inter;

		bool solutionIsUnique() const;
	};


	Segment(const RationalPoint& p1, const RationalPoint&);
	Segment(const Segment& other);
	Segment(Segment&& other);
	Segment& operator=(const Segment& other);
	Segment& operator=(Segment&& other);
	~Segment();

	friend bool operator==(const Segment& lhs, const Segment& rhs);
	friend bool operator!=(const Segment& lhs, const Segment& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Segment& s);

	bool isAlignedWith(const Rational2DPoint& X) const;
	bool isAlignedWith(const RationalPoint& X) const;
	bool segmentContains(const Rational2DPoint& X) const;
	bool segmentContains(const RationalPoint& X) const;
	bool isParallelTo(const Segment& S) const;
	Rational2DPoint directorVector() const;
	RationalPoint intersectionWith(const Segment& S) const;
	bool hasAsExtremity(const Rational2DPoint& X) const;
	bool hasAsExtremity(const RationalPoint& X) const;
	IntersectSol uniqueIntersectionWith(const Segment& S) const;
	Rational lambdaCoeff(const Rational2DPoint& X) const;

	RationalPoint p1;
	RationalPoint p2;
};

class SplitSegmentWrapper {

	friend class IntersectionManager;

public:
	SplitSegmentWrapper(const Segment& base, SplitSegmentWrapper* father = nullptr);
	SplitSegmentWrapper(const SplitSegmentWrapper& seg);
	~SplitSegmentWrapper();

	friend bool operator==(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs);
	friend bool operator!=(const SplitSegmentWrapper& lhs, const SplitSegmentWrapper& rhs);
	friend std::ostream& operator<<(std::ostream& os, const SplitSegmentWrapper& sW);

	class IncorrectSplit : std::exception {
	};

	/*
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
	*/
	size_t size() const;

	Segment::IntersectSol intersectionWith(const SplitSegmentWrapper& s) const;

	void split(const RationalPoint& splitPoint);
	bool isSplit() const;
	bool isRoot() const;

private: // Methods

	void __checkValidity() const;

private: // Members
	bool __isSplit;
	Segment base;
	SplitSegmentWrapper* father;
	SplitSegmentWrapper* son1;
	SplitSegmentWrapper* son2;
};


class PolyLineCurve {

	friend class iterator;

public:

	class PolyLineCurveOutOfRangeIteratorException : std::exception {
	};

	class iterator : public std::iterator<std::input_iterator_tag, SplitSegmentWrapper> {
		friend class PolyLineCurve;
	public:
		iterator(const iterator& it);
		iterator& operator=(const iterator& it);
		~iterator();

		iterator& operator++();
		iterator operator++(int);
		friend bool operator==(const iterator& lhs, const iterator& rhs);
		friend bool operator!=(const iterator& lhs, const iterator& rhs);
		SplitSegmentWrapper& operator*() const;

	private:
		explicit iterator(std::vector<SplitSegmentWrapper>& curve);
		//void pointEnd();

	private: // Members
		std::vector<SplitSegmentWrapper>& curve;
		std::vector<SplitSegmentWrapper>::iterator internalIt;
		//SplitSegmentWrapper::iterator* subInternalIt;
	};

	explicit PolyLineCurve(std::vector<SplitSegmentWrapper> segs);
	explicit PolyLineCurve(const std::vector<Segment>& segs);

	iterator begin();
	iterator end();
	size_t size() const;

private:
	std::vector<SplitSegmentWrapper> segments;
};


class IntersectionManager {
	friend class Segment;

	class GraphGenerationException : public std::exception {};

public:
	IntersectionManager();
	~IntersectionManager();

	bool areEqual(const RationalPoint& r1, const RationalPoint& r2) const;
	unsigned int requestPoint(const RationalPoint& p);
	Graph generateGraph();


private:
	std::vector<RationalPoint> intersectionPointsSet;
	std::vector<PolyLineCurve> allCurves;
	SettingWindow* window;
};
