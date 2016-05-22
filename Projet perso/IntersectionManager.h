#pragma once
#include "Rational2DPoint.h"
#include "InternalPositionSystem.h"
#include "Graph.h"

#include <iterator>
#include <functional>

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
		SplitSegmentWrapper& operator*() const;;

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

	unsigned int requestPoint(const RationalPoint& p, std::function<bool(const RationalPoint& p1, const RationalPoint& p2)> equal = std::mem_fn(&RationalPoint::operator==));
	Graph generateGraph() {

		std::vector<SplitSegmentWrapper> allSegs;
		// Regroupment of all the segment founds
		for (PolyLineCurve& PLC : allCurves) {
			for (SplitSegmentWrapper& SSW : PLC) {
				allSegs.push_back(SSW);
			}
		}
		// We compute all the intersections
		for (auto i = 0; i < allSegs.size() - 1; ++i) {
			for (auto j = i + 1; j < allSegs.size(); ++j) {
				Segment::IntersectSol sol = allSegs[i].intersectionWith(allSegs[j]);
				if (sol.solutionIsUnique()) {
					allSegs[i].split(sol.inter);
					allSegs[j].split(sol.inter);
				} else {
					if (sol.exists && !sol.isUnique) {
						throw GraphGenerationException();
					}
				}
			}
		}
	

		Graph G(intersectionPointsSet.size());
		std::function<void(const SplitSegmentWrapper& SSW, Graph& G)> fun = [&](const SplitSegmentWrapper& SSW, Graph& G)
		{
			if (SSW.isSplit()) {
				fun(*(SSW.son1), G);
				fun(*(SSW.son2), G);
			} else {
				unsigned int ind1 = requestPoint(SSW.base.p1);
				unsigned int ind2 = requestPoint(SSW.base.p2);
				G.addEdge(ind1, ind2);
			}
		};

		return G;

	}


private:
	std::vector<RationalPoint> intersectionPointsSet;
	std::vector<PolyLineCurve> allCurves;
};
