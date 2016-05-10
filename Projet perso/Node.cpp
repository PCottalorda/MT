#include "Node.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>


//=============================== COUNTER ===================================================================

class Counter {
private:
	std::vector<bool> loopGest;
	uint64_t size;
	std::function<bool(const std::vector<bool>&, uint64_t)> func;

public:
	Counter(uint64_t size, const std::function<bool(const std::vector<bool>&, uint64_t)>& func);
	~Counter();
	std::vector<bool> getNext();
	bool finished() const;
	std::vector<std::vector<bool>> generateAll();

private:
	uint64_t state();
	void reset();
	void __next(uint64_t i);
	void findNext();
	void next();

};

class BinomialConfigs : public Counter {
public:
	BinomialConfigs(uint64_t n, uint64_t m) :
		Counter(m, [n, m](const std::vector<bool>& v, uint64_t s) {
			        uint64_t count = 0;
			        for (uint64_t i = 0; i < s; i++)
				        if (v[i]) count++;

			        return count == n;
		        }) {
		assert(m >= n);
	};
};

class AllConfigs : public Counter {
public:
	AllConfigs(uint64_t s) :
		Counter(s, [](const std::vector<bool>&, uint64_t) {
			        return true;
		        }) {
	};
};

Counter::Counter(uint64_t size, const std::function<bool(const std::vector<bool>&, uint64_t)>& func) : size(size),
                                                                                                       func(func),
                                                                                                       loopGest(size + 1, false) {

}

Counter::~Counter() {
}

uint64_t Counter::state() {
	uint64_t res = 0;
	for (int64_t i = loopGest.size() - 1; i >= 0; i--) {
		res *= 2;
		if (loopGest[i]) {
			res++;
		}
	}
	return res;
}


std::vector<bool> Counter::getNext() {
	std::cout << "\tConfiguration Found: " << state() << std::endl;
	std::vector<bool> res(size);
	for (uint64_t i = 0; i < size; i++) {
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
	for (uint64_t i = 0; i < size + 1; i++) {
		loopGest[i] = false;
	}
	findNext();
}

void Counter::__next(uint64_t i) {
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
	std::cout << "Generate possible eulerian orientation of node " << internalNumber << std::endl;
	std::vector<OrientationOnNode> orientations;

	std::vector<Edge> lockedEdges;
	std::vector<Edge> notLockedLoopedEdges;
	std::vector<Edge> notLockedNotLoopedEdges;

	uint64_t degree = 0;
	uint64_t in_degree = 0;
	uint64_t out_degree = 0;

	std::for_each(edges.begin(), edges.end(), [&degree](const Edge* ed) {
		              if (!ed->isLoop()) degree++;
	              });

	std::cout << "Non looped degree: " << degree << std::endl;

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

	std::cout << "Non looped locked in degree  :" << in_degree << std::endl;
	std::cout << "Non looped locked out degree :" << out_degree << std::endl;

	assert(edges.size() == lockedEdges.size() + notLockedLoopedEdges.size() + notLockedNotLoopedEdges.size());
	assert(degree % 2 == 0);
	assert(degree >= out_degree + in_degree);

	// uint64_t is of degree 2.
	uint64_t demi_degree = degree / 2;

	if ((in_degree > demi_degree) || (out_degree > demi_degree)) {
		// No valid orientation
		assert(orientations.empty());
		return orientations;
	} else {
		uint64_t in_rem = demi_degree - in_degree;
		uint64_t out_rem = demi_degree - out_degree;

		assert(in_rem + out_rem == notLockedNotLoopedEdges.size());

		std::cout << "Generate Binomial configs (" << in_rem << " parmi " << notLockedNotLoopedEdges.size() << ")..." << std::endl;
		std::vector<std::vector<bool>> notLoopedStates = BinomialConfigs(in_rem, notLockedNotLoopedEdges.size()).generateAll();
		std::cout << notLoopedStates.size() << " configuration(s) found" << std::endl;
		std::cout << "Generate all configs..." << std::endl;
		std::vector<std::vector<bool>> loopedStates = AllConfigs(notLockedLoopedEdges.size()).generateAll();
		std::cout << loopedStates.size() << " configuration(s) found" << std::endl;

		//--- Lambdas ---
		auto applyInChange = [&](std::vector<bool>& setOri, std::vector<Edge>& edges) {
			assert(setOri.size() == edges.size());
			for (uint64_t i = 0; i < setOri.size(); i++) {
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
			for (uint64_t i = 0; i < setOri.size(); i++) {
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
			std::cout << "No loop found..." << std::endl;
			for (uint64_t i = 0; i < notLoopedStates.size(); i++) {
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
			std::cout << "Only loops found..." << std::endl;
			for (uint64_t j = 0; j < loopedStates.size(); j++) {
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
			std::cout << "Loops and other edges found..." << std::endl;
			for (uint64_t i = 0; i < notLoopedStates.size(); i++) {
				for (uint64_t j = 0; j < loopedStates.size(); j++) {
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

	for (uint64_t i = 0; i < edges.size(); i++) {
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
