#include "Node.h"

#include <algorithm>
#include <cassert>

Node::Node(int i) : internalNumber(i), marked(false) {
}

Node::~Node() {
}


void Node::addAdjacentEdge(Edge* ed) {
	if (!ed->isAdjacentTo(this))
		throw std::exception();

	// Architecture can be optimized!
	edges.push_back(ed);
	std::sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2){return e1 < e2; });
}

void Node::setMarked(bool mark) {
	marked = mark;
}

bool Node::getMarked() const {
	return marked;
}

void Node::mark() {
	marked = true;
}

void Node::unMark() {
	marked = false;
}

void Node::__markage_proc_stongly_connected() {
	if (!marked) {
		marked = true;
		for each (const Edge* ed in edges) {
			if (ed->getOrigin() == this)
				ed->getDestination()->__markage_proc_stongly_connected();
		}
	}
}

void Node::__markage_proc_weakly_connected() {
	if (!marked) {
		marked = true;
		for each (Edge* e in edges) {
			e->Node1->__markage_proc_weakly_connected();
			e->Node2->__markage_proc_weakly_connected();
		}
	}
}

bool Node::__check_validity() {
	return std::all_of(edges.begin(), edges.end(), &Edge::isAdjacentTo);
}

std::vector<OrientationOnNode> Node::allPossibleOrientations() const {
	std::vector<OrientationOnNode> orientations;

	std::vector<Edge> lockedEdges;
	std::vector<Edge> notLockedLoopedEdges;
	std::vector<Edge> notLockedNotLoopedEdges;

	uint64_t degree = 0;
	uint64_t in_degree = 0;
	uint64_t out_degree = 0;

	std::for_each(edges.begin(), edges.end(), [&degree](const Edge& ed) {
		              if (ed.isLoop()) degree++;
	              });

	// We dispatch the edges.
	for each (Edge* ed in edges) {
		Edge& OriEdge = *ed;
		if (OriEdge.isLocked()) {
			lockedEdges.push_back(OriEdge);
		} else {
			if (OriEdge.isLoop()) {
				notLockedLoopedEdges.push_back(OriEdge);
			} else {
				notLockedNotLoopedEdges.push_back(OriEdge);
				if (OriEdge.getOrigin() == this) {
					out_degree++;
				}
				if (OriEdge.getDestination() == this) {
					in_degree++;
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

			Counter binGen(in_rem, [in_rem,out_rem](const std::vector<bool>& v, uint64_t s) {
				               uint64_t count = 0;
				               for (uint64_t i = 0; i < s; i++) {
					               if (v[i]) count++;
				               }
				               return count == in_rem + out_rem;
			               });
			std::vector<std::vector<bool>> notLoopedStates = binGen.generateAll();

			Counter allGen(notLockedLoopedEdges.size(), [](const std::vector<bool>&, uint64_t) {
				               return true;
			               });
			std::vector<std::vector<bool>> loopedStates = allGen.generateAll();

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

			std::vector<Edge> loops(notLockedLoopedEdges);
			std::vector<Edge> othersEdges(notLockedNotLoopedEdges);

			std::vector<Edge> resEdges;

			if (loopedStates.empty()) {
				// There is no loop
				assert(notLoopedStates.size() == othersEdges.size());
				for (uint64_t i = 0; i < notLoopedStates.size(); i++) {
					resEdges.clear();
					applyInChange(notLoopedStates[i], othersEdges);
					resEdges = concatenate(resEdges, othersEdges);
					std::for_each(resEdges.begin(), resEdges.end(), [](OrientedEdgeWrapper& oriEdge) {
						              oriEdge.lock();
					              });
					resEdges = concatenate(resEdges, lockedEdges);
					Node oriNode(baseNode);
					oriNode.setOrientedEdges(resEdges);
					assert(oriNode.isComplete());
				}
			} else if (notLoopedStates.empty()) {
				// There is nothing else but loop
				assert(loopedStates.size() == othersEdges.size());
				for (uint64_t j = 0; j < loopedStates.size(); j++) {
					resEdges.clear();
					applyInChange(loopedStates[j], othersEdges);
					resEdges = concatenate(resEdges, othersEdges);
					std::for_each(resEdges.begin(), resEdges.end(), [](OrientedEdgeWrapper& oriEdge) {
						              oriEdge.lock();
					              });
					resEdges = concatenate(resEdges, lockedEdges);
					Node oriNode(baseNode);
					oriNode.setOrientedEdges(resEdges);
					assert(oriNode.isComplete());
				}
			} else {
			}
		}
	}
}

void Node::setOrientedEdges(OrientationOnNode& ori) {
	assert(std::is_sorted(edges.begin(),edges.end(), [](const Edge* e1, const Edge* e2){return e1->id < e2->id; }));
	assert(std::is_sorted(ori.begin(),ori.end()));
	assert(ori.size() == edges.size());

	for (uint64_t i = 0; i < edges.size(); i++) {
		assert(edges[i]->id == ori[i].id);
		*(edges[i]) = ori[i];
	}
}

bool Node::isComplete() const {
	return std::all_of(edges.begin(), edges.end(), &Edge::isLocked);
}

Counter::Counter(uint64_t size, const std::function<bool(const std::vector<bool>&, uint64_t)>& func): size(size),
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
	assert(std::all_of(allTrue.begin(),allTrue.end(),std::bind2nd(func,size)));
	return allTrue;
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
	return ((incommingEdges.size() + outcommingEdges.size()) % 2) == 0;
}

uint64_t OrientationOnNode::compute_in_mask() {
	uint64_t mask = 0;
	for (size_t i = 0; i < edges.size(); i++) {
		mask <<= 1;
		if (edges[i]->isFixed() && (this == edges[i]->getTail())) {
			mask += 1;
		}
	}
	return mask;
}

uint64_t OrientationOnNode::compute_out_mask() {
	uint64_t mask = 0;
	for (size_t i = 0; i < edges.size(); i++) {
		mask <<= 1;
		if (edges[i]->isFixed() && (this == edges[i]->getHead())) {
			mask += 1;
		}
	}
	return mask;
}

std::vector<uint64_t> OrientationOnNode::possibleOrientations() {
	std::vector<uint64_t> out;
	uint64_t in_mask = compute_in_mask();
	uint64_t out_mask = compute_out_mask();
	//=== TODO: Extends!!! =====
	uint64_t possibilities[6] = {3, 5, 6, 9, 10, 12};

	for (int i = 0; i < 6; i++) {
		uint64_t poss = possibilities[i];
		if ((out_mask & poss) != out_mask)
			continue;

		if ((in_mask & ~poss) != in_mask)
			continue;

		out.push_back(poss);
	}
	//==========================

	return out;
}

void OrientationOnNode::setOrientation(uint64_t ori) {
	for (size_t i = 0; i < edges.size(); i++) {
		edges[i]->setOrientation((ori & 0x1) == 0, this);
		ori >>= 1;
	}
}

uint64_t OrientationOnNode::compute_reset_mask() {
	uint64_t mask = 0;
	for (size_t i = 0; i < edges.size(); i++) {
		mask <<= 1;
		if (edges[i]->isFixed()) {
			mask += 1;
		}
	}
	return mask;
}

void OrientationOnNode::reset(uint64_t mask) {
	for (size_t i = 0; i < edges.size(); i++) {
		if ((mask & 0x1) == 0) { // mask % 2 == 0
			edges[i]->reset();
		}
		mask >>= 1;
	}
}

void OrientationOnNode::recompute_orientation_edges() {
	incommingEdges.clear();
	outcommingEdges.clear();

	for each (Edge* ed in edges) {
		if (ed->getHead() == this) {
			outcommingEdges.push_back(ed);
		} else {
			incommingEdges.push_back(ed);
		}
	}
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
			count += 1;
		}
	}
	return count;
}