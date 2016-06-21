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
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>


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



class BinomialConfigs : public Counter {
public:
	class BinomialEntry {
	public:
		size_t n;
		size_t m;
	
		BinomialEntry(size_t n = 1, size_t m = 1) : n(n), m(m) {};

	};
public:
	BinomialConfigs(BinomialEntry en) :
		Counter(en.m, [&en](const std::vector<bool>& v, size_t s) {
			        size_t count = 0;
			        for (size_t i = 0; i < s; i++)
				        if (v[i]) count++;

			        return count == en.n;
		        }) {
		if (en.m < en.n) {
			std::string errStr = "Invalid entry found for BinomialConfigs: (" + std::to_string(en.n) + "," + std::to_string(en.m) + ")";
			throw std::out_of_range(errStr);
		}
	};
};

// Definition of a hash function for BinomialEntry
namespace std {
	template<>
	struct hash<BinomialConfigs::BinomialEntry> {
		std::size_t operator()(const BinomialConfigs::BinomialEntry& Key) const {
			return Key.n*Key.m + Key.n;
		}
	};
}

class AllConfigs : public Counter {
public:
	AllConfigs(size_t s) :
		Counter(s, [](const std::vector<bool>&, size_t) {
			        return true;
		        }) {
	};
};

Counter::Counter(size_t size, const std::function<bool(const std::vector<bool>&, size_t)>& func) : size(size),
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


template<typename Ctr, typename Key>
class CounterManager {
	// Type definition
private:
	using ResType = typename std::result_of < decltype(&Ctr::generateAll)(Ctr) >::type;

public:
	CounterManager() {};
	~CounterManager() {};
	/*
	const ResType& getSets (Key key) {
		// We check if we have already computed the results.
		try {
			auto result = computedResults[key];
			return result;
		} catch (std::out_of_range&) {	// The result was not already computed
			Ctr counter(key);
			ResType res = counter.generateAll();
			computedResults.insert(key, res);
			return computedResults[key];
		}
	}*/

	/*
	const ResType& getSets(Key key) {
		// We check if we have already computed the results.
		const auto it = computedResults.find(key);
		if (it == computedResults.end()) {
			// No element with such key have been found
			return *(computedResults.insert(std::make_pair<Key, ResType>(key, Ctr(key).generateAll())));
		} else {
			return *it;
		}
	}
	*/

private:
	//std::unordered_map<Key, decltype(std::declval<Ctr>().generateAll())> computedResults;
	std::unordered_map<Key, ResType> computedResults;

};

using BinomialConfigsCounterManager = CounterManager < BinomialConfigs, BinomialConfigs::BinomialEntry > ;
using AllConfigsCounterManager = CounterManager < AllConfigs, size_t > ;




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
	static BinomialConfigsCounterManager BinomialConfsMan;
	static AllConfigsCounterManager AllConfsMan;


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

		BinomialConfigs::BinomialEntry binomialKey(in_rem, notLockedNotLoopedEdges.size());
		const std::vector<std::vector<bool>> notLoopedStates = BinomialConfigs(binomialKey).generateAll();
		//const std::vector<std::vector<bool>>& notLoopedStates = BinomialConfsMan.getSets(binomialKey);
		std::vector<std::vector<bool>> loopedStates = AllConfigs(notLockedLoopedEdges.size()).generateAll();
		//const std::vector<std::vector<bool>>& loopedStates = AllConfsMan.getSets(notLockedLoopedEdges.size());
		//auto loopedStates = AllConfsMan.getSets(notLockedLoopedEdges.size());

		//--- Lambdas ---
		auto applyInChange = [&](const std::vector<bool>& setOri, std::vector<Edge>& edges) {
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
		auto applyChange = [&](const std::vector<bool>& setOri, std::vector<Edge>& edges) {
			assert(setOri.size() == edges.size());
			for (size_t i = 0; i < setOri.size(); i++) {
				if (setOri[i]) {
					edges[i].reverseOrientation();
				}
			}
		};
		auto concatenate = [](const std::vector<Edge>& v1, const std::vector<Edge>& v2) -> std::vector<Edge> {
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
