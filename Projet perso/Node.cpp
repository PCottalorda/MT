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
	void reset();
	void __next(uint64_t i);
	void findNext();
	void next();

};

class BinomialConfigs : public Counter {
public:
	BinomialConfigs(uint64_t n, uint64_t m) :
		Counter(m, [n, m](const std::vector<bool>& v, uint64_t s)
	{
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
	}) {};
};

Counter::Counter(uint64_t size, const std::function<bool(const std::vector<bool>&, uint64_t)>& func) : size(size),
func(func),
loopGest(size + 1, false) {
	findNext();
}

Counter::~Counter() {
}

std::vector<bool> Counter::getNext() {
	std::vector<bool> res(size);
	for (uint64_t i = 0; i < size; i++) {
		res[i] = loopGest[i];
	}
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
	if (!finished()) {
		if (loopGest[i]) {
			loopGest[i] = false;
			__next(++i);
		}
	}
}

bool Counter::finished() const {
	return loopGest[size];
}

std::vector<std::vector<bool>> Counter::generateAll() {
	std::vector<std::vector<bool>> allTrue;
	reset();
	while (!finished()) {
		allTrue.push_back(getNext());
	}
	//assert(std::all_of(allTrue.begin(), allTrue.end(), [&](const std::vector<bool> &v){return func(v, size); }));
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
	std::sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2){return e1 < e2; });
	assert(__check_validity());
}

bool Node::__check_validity() {
	return std::all_of(edges.begin(), edges.end(), [&](const Edge* e){return e->isAdjacentTo(this); })
		&& std::is_sorted(edges.begin(), edges.end(), [](const Edge* e1, const Edge* e2){ return *e1 < *e2; });
}

std::vector<OrientationOnNode> Node::allPossibleOrientations() const {
	std::vector<OrientationOnNode> orientations;

	std::vector<Edge> lockedEdges;
	std::vector<Edge> notLockedLoopedEdges;
	std::vector<Edge> notLockedNotLoopedEdges;

	uint64_t degree = 0;
	uint64_t in_degree = 0;
	uint64_t out_degree = 0;

	std::for_each(edges.begin(), edges.end(), [&degree](const Edge* ed) {
		              if (ed->isLoop()) degree++;
	              });

	// We dispatch the edges.
	for each (Edge* ed in edges) {
		Edge& OriEdge = *ed;
		if (OriEdge.isLocked()) {
			lockedEdges.push_back(OriEdge);
		}
		else {
			if (OriEdge.isLoop()) {
				notLockedLoopedEdges.push_back(OriEdge);
			}
			else {
				notLockedNotLoopedEdges.push_back(OriEdge);
				if (OriEdge.getOrigin() == this) {
					out_degree++;
				}
				if (OriEdge.getDestination() == this) {
					in_degree++;
				}
			}
		}
	}

		assert(edges.size() == lockedEdges.size() + notLockedLoopedEdges.size() + notLockedNotLoopedEdges.size());
		assert(degree % 2 == 0);
		assert(degree <= out_degree + in_degree);

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
			
			std::vector<std::vector<bool>> notLoopedStates = BinomialConfigs(in_rem, notLockedNotLoopedEdges.size()).generateAll();
			std::vector<std::vector<bool>> loopedStates = AllConfigs(notLockedLoopedEdges.size()).generateAll();

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
			auto concatenate = [](std::vector<Edge>& v1, std::vector<Edge>& v2) {
				std::vector<Edge> res(v1);
				for (uint64_t i = 0; i < v2.size(); i++) {
					v1.push_back(v2[i]);
				}
				return res;
			};
			//---------------

			std::vector<Edge> resEdges;

			if (loopedStates.empty()) {
				// There is no loop
				for (uint64_t i = 0; i < notLoopedStates.size(); i++) {
					resEdges.clear();
					std::vector<Edge> loops(notLockedLoopedEdges);
					std::vector<Edge> othersEdges(notLockedNotLoopedEdges);
					assert(notLoopedStates.size() == othersEdges.size());
					applyInChange(notLoopedStates[i], othersEdges);
					resEdges = concatenate(resEdges, othersEdges);
					std::for_each(resEdges.begin(), resEdges.end(), std::mem_fn(&Edge::lock));
					resEdges = concatenate(resEdges, lockedEdges);
					assert(std::all_of(resEdges.begin(),resEdges.end(), std::mem_fn(&Edge::isLocked)));
					std::sort(resEdges.begin(), resEdges.end());
					assert(resEdges.size() == edges.size());
					orientations.push_back(resEdges);
				}
			} else if (notLoopedStates.empty()) {
				// There is nothing else but loop

				for (uint64_t j = 0; j < loopedStates.size(); j++) {
					resEdges.clear();
					std::vector<Edge> loops(notLockedLoopedEdges);
					std::vector<Edge> othersEdges(notLockedNotLoopedEdges);
					assert(loopedStates.size() == loops.size());
					applyChange(loopedStates[j], othersEdges);
					resEdges = concatenate(resEdges, loops);
					std::for_each(resEdges.begin(), resEdges.end(), std::mem_fn(&Edge::lock));
					resEdges = concatenate(resEdges, lockedEdges);
					assert(std::all_of(resEdges.begin(), resEdges.end(), std::mem_fn(&Edge::isLocked)));
					std::sort(resEdges.begin(), resEdges.end());
					assert(resEdges.size() == edges.size());
					orientations.push_back(resEdges);
				}
			} else {
				for (uint64_t i = 0; i < notLoopedStates.size(); i++) {
					for (uint64_t j = 0; j < loopedStates.size(); j++) {
						resEdges.clear();
						std::vector<Edge> loops(notLockedLoopedEdges);
						std::vector<Edge> othersEdges(notLockedNotLoopedEdges);
						assert(notLoopedStates.size() == othersEdges.size());
						assert(loopedStates.size() == loops.size());
						applyInChange(notLoopedStates[i], othersEdges);
						applyChange(loopedStates[j], loops);
						resEdges = concatenate(resEdges, othersEdges);
						resEdges = concatenate(resEdges,loops);
						std::for_each(resEdges.begin(), resEdges.end(), std::mem_fn(&Edge::lock));
						resEdges = concatenate(resEdges, lockedEdges);
						assert(std::all_of(resEdges.begin(), resEdges.end(), std::mem_fn(&Edge::isLocked)));
						std::sort(resEdges.begin(), resEdges.end());
						orientations.push_back(resEdges);
					}
				}
			}
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
	OrientationOnNode res(edges.size());
	for each (const Edge* ed in edges) {
		res.push_back(*ed);
	}
	return res;
}

bool Node::isComplete() const {
	return std::all_of(edges.begin(), edges.end(), std::mem_fn(&Edge::isLocked));
}


bool Node::isEulerian() const {
	uint64_t out(0), in(0);
	for each(const Edge* ed in edges) {
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
		}
		else {
			count++;
		}
	}
	return count;
}

