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


#include "Node.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <unordered_map>


//=============================== COUNTER ===================================================================

class Counter {
private:
	std::vector<bool> loopGest;
	size_t size;
	std::function<bool(const std::vector<bool>&, size_t)> func;

public:
	Counter(size_t size, const std::function<bool(const std::vector<bool>&, size_t)>& func);
	~Counter();
	std::vector<bool> getNext();
	bool finished() const;
	std::vector<std::vector<bool>> generateAll();

private:
	size_t state();
	void reset();
	void __next(size_t i);
	void findNext();
	void next();

};



Counter::Counter(size_t size, const std::function<bool(const std::vector<bool>&, size_t)>& func) : 
	size(size),
	func(func),
	loopGest(size + 1, false) {

}

Counter::~Counter() {
}

size_t Counter::state() {
	size_t res = 0;
	for (int i = loopGest.size() - 1; i >= 0; i--) {
		res *= 2;
		if (loopGest[i]) {
			res++;
		}
	}
	return res;
}


std::vector<bool> Counter::getNext() {
	std::vector<bool> res(size);
	for (size_t i = 0; i < size; i++) {
		res[i] = loopGest[i];
	}
	next();
	findNext();
	return res;
}

void Counter::findNext() {
	while (!finished() && !func(loopGest, size)) {
		next();
	}
}

void Counter::next() {
	__next(0);
}


void Counter::reset() {
	for (size_t i = 0; i < size + 1; i++) {
		loopGest[i] = false;
	}
	findNext();
}

void Counter::__next(size_t i) {
	if (i < loopGest.size()) {
		if (loopGest[i]) {
			loopGest[i] = false;
			__next(i + 1);
		} else {
			loopGest[i] = true;
		}
	}
}

bool Counter::finished() const {
	return loopGest[size];
}

std::vector<std::vector<bool>> Counter::generateAll() {
	std::vector<std::vector<bool>> allTrue;
	reset();
	if (loopGest.size() != 1) {
		while (!finished()) {
			allTrue.push_back(getNext());
		}
	}
	assert(std::all_of(allTrue.begin(), allTrue.end(), [&](const std::vector<bool> &v){return func(v, size); }));
	return allTrue;
}




struct BinomialParams {
	size_t n;
	size_t m;

	BinomialParams(size_t n, size_t m) : n(n), m(m) {};

	friend bool operator==(const BinomialParams& lhs, const BinomialParams& rhs) {
		return lhs.n == rhs.n
			&& lhs.m == rhs.m;
	}

	friend bool operator!=(const BinomialParams& lhs, const BinomialParams& rhs) {
		return !(lhs == rhs);
	}
};

namespace std {
	template<>
	struct hash<BinomialParams> {
		std::size_t operator() (const BinomialParams& b) const {
			return b.m*b.n + b.n;
		}
	};
}

class BinomialConfigs : public Counter {
public:
	BinomialConfigs(size_t n, size_t m) :
		Counter(m, [n, m](const std::vector<bool>& v, size_t s) {
		size_t count = 0;
		for (size_t i = 0; i < s; i++)
			if (v[i]) count++;

		return count == n;
	}) {
		assert(m >= n);
	}

	BinomialConfigs(BinomialParams p) : BinomialConfigs(p.n,p.m) {}
};

class AllConfigs : public Counter {
public:
	AllConfigs(size_t s) :
		Counter(s, [](const std::vector<bool>&, size_t) {
		return true;
	}) {
	};
};


template<typename Type, typename Key>
class CounterValuesManager {
	using ResType = typename std::result_of<decltype(&Type::generateAll)(Type)>::type;
	using MapType = std::unordered_map<Key, ResType>;
public:
	CounterValuesManager() {};
	~CounterValuesManager() {};


	const ResType& getValue(Key key) {
		auto it = computedResults.find(key);
		if (it == computedResults.end()) {
			ResType res = Type(key).generateAll();
			auto resInsertion = computedResults.insert(std::pair<Key, ResType>(key, res));
			return resInsertion.first->second;
		} else {
			return it->second;
		}
	}

private:
	MapType computedResults;
};


using BinomialValuesManager = CounterValuesManager < BinomialConfigs, BinomialParams > ;
using AllValuesManager = CounterValuesManager < AllConfigs, size_t > ;




//======== NODE ==============================================================================================

Node::Node(int i) : internalNumber(i) {
}

Node::~Node() {
}

void Node::addAdjacentEdge(Edge* ed) {
	if (!ed->isAdjacentTo(this))
		throw std::exception();

	// Architecture can be optimized!
	edges.push_back(ed);
	std::sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2) {
		          return *e1 < *e2;
	          });
	__check_validity();
}

void Node::__check_validity() {
	assert(std::all_of(edges.begin(), edges.end(), [&](const Edge* e){return e->isAdjacentTo(this); }));
	assert(std::is_sorted(edges.begin(), edges.end(), [](const Edge* e1, const Edge* e2){ return *e1 < *e2; }));
}

std::vector<OrientationOnNode> Node::allPossibleOrientations() const {
	//std::cout << "Generate possible eulerian orientation of node " << internalNumber << std::endl;

	BinomialValuesManager	BinValMan;
	AllValuesManager		AllValMan;

	std::vector<OrientationOnNode> orientations;

	std::vector<Edge> lockedEdges;
	std::vector<Edge> notLockedLoopedEdges;
	std::vector<Edge> notLockedNotLoopedEdges;

	size_t degree = 0;
	size_t in_degree = 0;
	size_t out_degree = 0;

	std::for_each(edges.begin(), edges.end(), [&degree](const Edge* ed) {
		              if (!ed->isLoop()) degree++;
	              });

	//std::cout << "Non looped degree: " << degree << std::endl;

	// We dispatch the edges.
	for each (Edge* ed in edges) {
		Edge& OriEdge = *ed;
		if (OriEdge.isLocked()) {
			lockedEdges.push_back(OriEdge);
			if (!OriEdge.isLoop()) {
				if (OriEdge.getOrigin() == this) {
					out_degree++;
				}
				if (OriEdge.getDestination() == this) {
					in_degree++;
				}
			}
		} else {
			if (OriEdge.isLoop()) {
				notLockedLoopedEdges.push_back(OriEdge);
			} else {
				notLockedNotLoopedEdges.push_back(OriEdge);
			}
		}
	}

	//std::cout << "Non looped locked in degree  :" << in_degree << std::endl;
	//std::cout << "Non looped locked out degree :" << out_degree << std::endl;

	assert(edges.size() == lockedEdges.size() + notLockedLoopedEdges.size() + notLockedNotLoopedEdges.size());
	assert(degree % 2 == 0);
	assert(degree >= out_degree + in_degree);

	// size_t is of degree 2.
	size_t demi_degree = degree / 2;

	if ((in_degree > demi_degree) || (out_degree > demi_degree)) {
		// No valid orientation
		assert(orientations.empty());
		return orientations;
	} else {
		size_t in_rem = demi_degree - in_degree;
		size_t out_rem = demi_degree - out_degree;

		assert(in_rem + out_rem == notLockedNotLoopedEdges.size());

		BinomialParams binParams(in_rem, notLockedNotLoopedEdges.size());
		std::vector<std::vector<bool>> notLoopedStates2 = BinomialConfigs(binParams).generateAll();
		std::vector<std::vector<bool>> notLoopedStates3 = BinValMan.getValue(binParams);
		std::vector<std::vector<bool>> notLoopedStates = BinomialConfigs(in_rem, notLockedNotLoopedEdges.size()).generateAll();
		assert(notLoopedStates == notLoopedStates2);
		assert(notLoopedStates2 == notLoopedStates3);
		std::vector<std::vector<bool>> loopedStates = AllConfigs(notLockedLoopedEdges.size()).generateAll();

		//--- Lambdas ---
		auto applyInChange = [&](std::vector<bool>& setOri, std::vector<Edge>& edges) {
			assert(setOri.size() == edges.size());
			for (size_t i = 0; i < setOri.size(); i++) {
				assert(edges[i].isAdjacentTo(this));
				if (setOri[i]) {
					if (edges[i].getDestination() != this) {
						edges[i].reverseOrientation();
					}
				} else {
					if (edges[i].getDestination() == this) {
						edges[i].reverseOrientation();
					}
				}
			}
		};
		auto applyChange = [&](std::vector<bool>& setOri, std::vector<Edge>& edges) {
			assert(setOri.size() == edges.size());
			for (size_t i = 0; i < setOri.size(); i++) {
				if (setOri[i]) {
					edges[i].reverseOrientation();
				}
			}
		};
		auto concatenate = [](std::vector<Edge>& v1, std::vector<Edge>& v2) -> std::vector<Edge> {
			std::vector<Edge> res;
			std::for_each(v1.begin(), v1.end(), [&res](const Edge& e) {
				              res.push_back(e);
			              });
			std::for_each(v2.begin(), v2.end(), [&res](const Edge& e) {
				              res.push_back(e);
			              });
			return res;
		};
		//---------------

		std::vector<Edge> resEdges;

		assert(orientations.empty());

		if (loopedStates.empty()) {
			// There is no loop
			//std::cout << "No loop found..." << std::endl;
			for (size_t i = 0; i < notLoopedStates.size(); i++) {
				resEdges.clear();
				std::vector<Edge> othersEdges(notLockedNotLoopedEdges);
				assert(notLoopedStates[i].size() == othersEdges.size());
				applyInChange(notLoopedStates[i], othersEdges);
				resEdges = concatenate(resEdges, othersEdges);
				resEdges = concatenate(resEdges, lockedEdges);
				std::sort(resEdges.begin(), resEdges.end());
				assert(resEdges.size() == edges.size());
				orientations.push_back(resEdges);
			}
		} else if (notLoopedStates.empty()) {
			// There is nothing else but loop
			//std::cout << "Only loops found..." << std::endl;
			for (size_t j = 0; j < loopedStates.size(); j++) {
				resEdges.clear();
				std::vector<Edge> loops(notLockedLoopedEdges);
				assert(loopedStates[j].size() == loops.size());
				applyChange(loopedStates[j], loops);
				resEdges = concatenate(resEdges, loops);
				resEdges = concatenate(resEdges, lockedEdges);
				std::sort(resEdges.begin(), resEdges.end());
				assert(resEdges.size() == edges.size());
				orientations.push_back(resEdges);
			}
		} else {
			//std::cout << "Loops and other edges found..." << std::endl;
			for (size_t i = 0; i < notLoopedStates.size(); i++) {
				for (size_t j = 0; j < loopedStates.size(); j++) {
					resEdges.clear();
					std::vector<Edge> loops(notLockedLoopedEdges);
					std::vector<Edge> othersEdges(notLockedNotLoopedEdges);
					assert(notLoopedStates[i].size() == othersEdges.size());
					assert(loopedStates[j].size() == loops.size());
					applyInChange(notLoopedStates[i], othersEdges);
					applyChange(loopedStates[j], loops);
					resEdges = concatenate(resEdges, othersEdges);
					resEdges = concatenate(resEdges, loops);
					resEdges = concatenate(resEdges, lockedEdges);
					std::sort(resEdges.begin(), resEdges.end());
					assert(resEdges.size() == edges.size());
					orientations.push_back(resEdges);
				}
			}
		}
		assert(!orientations.empty());
		std::for_each(orientations.begin(), orientations.end(), [](OrientationOnNode& o) {
			              std::for_each(o.begin(), o.end(), std::mem_fn(&Edge::lock));
		              });
		assert(std::all_of(orientations.begin(), orientations.end(), [](const OrientationOnNode& o)
			{
				return std::all_of(o.begin(), o.end(), std::mem_fn(&Edge::isLocked));
			}));
		return orientations;
	}
}


void Node::setOrientedEdges(const OrientationOnNode& ori) {
	assert(std::is_sorted(edges.begin(),edges.end(), [](const Edge* e1, const Edge* e2){return e1->id < e2->id; }));
	assert(std::is_sorted(ori.begin(),ori.end()));
	assert(ori.size() == edges.size());

	for (size_t i = 0; i < edges.size(); i++) {
		assert(edges[i]->id == ori[i].id);
		*(edges[i]) = ori[i];
	}
}

OrientationOnNode Node::saveOrientation() const {
	OrientationOnNode res;
	assert(std::is_sorted(edges.begin(), edges.end(), [](const Edge* e1, const Edge* e2){return *e1 < *e2; }));
	for each (const Edge* e in edges) {
		res.push_back(*e);
	}
	assert(std::is_sorted(res.begin(), res.end()));
	return res;
}

bool Node::isComplete() const {
	return std::all_of(edges.begin(), edges.end(), std::mem_fn(&Edge::isLocked));
}


bool Node::isEulerian() const {
	uint64_t out(0), in(0);
	for each (const Edge* ed in edges) {
		if (ed->getDestination() == this) in++;
		if (ed->getOrigin() == this) out++;
	}
	return in == out;
}

bool Node::isWeaklyEulerian() const {
	return (degree() % 2) == 0;
}


int Node::getInternalNumber() const {
	return internalNumber;
}


int Node::degree() const {
	int count = 0;
	for (size_t i = 0; i < edges.size(); i++) {
		std::cerr << " : " << *(edges[i]) << " : " << std::endl;
		if (edges[i]->isLoop()) {
			count += 2;
		} else {
			count++;
		}
	}
	return count;
}
