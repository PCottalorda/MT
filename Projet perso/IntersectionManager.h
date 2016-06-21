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


#include <exception>
#include <set>

#include "HomologieValue.h"

class Graph;
class SettingWindow;

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

	private: // Members
		std::vector<SplitSegmentWrapper>& curve;
		std::vector<SplitSegmentWrapper>::iterator internalIt;
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
	IntersectionManager(const SettingWindow*);
	~IntersectionManager();

	bool areEqual(const RationalPoint& r1, const RationalPoint& r2) const;
	unsigned int requestPoint(const RationalPoint& p);
	std::set<HomologieValue> generateValues();
	size_t size() const {
		return intersectionPointsSet.size();
	}

private:
	std::vector<RationalPoint> intersectionPointsSet;
	std::vector<PolyLineCurve> allCurves;
	const SettingWindow* window;
};
