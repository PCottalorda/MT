#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include "Edge.h"


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

class Node {
	friend class Graph;
	friend class Edge;
	friend class OrientationOnNode;

public:
	Node(int i);
	~Node();

	int degree() const;
	void addAdjacentEdge(Edge* ed);
	void setMarked(bool mark);
	bool getMarked() const;
	void mark();
	void unMark();
	bool isEulerian() const;
	bool isWeaklyEulerian() const;
	bool isFixed();
	uint64_t compute_in_mask();
	uint64_t compute_out_mask();
	// in = 0, out = 1
	std::vector<uint64_t> possibleOrientations();
	// in = 0, out = 1
	void setOrientation(uint64_t ori);
	uint64_t compute_reset_mask();
	void reset(uint64_t mask);
	void recompute_orientation_edges();
	int getInternalNumber() const;

	void __markage_proc_stongly_connected();
	void __markage_proc_weakly_connected();
	bool __check_validity();

private:
	std::vector<Edge*> edges;

	bool marked;
	int internalNumber;
};

class OrientationOnNode {
	Node* baseNode;
	std::vector<OrientedEdgeWrapper> orientedEdges;

	explicit OrientationOnNode(Node* base_node);
	~OrientationOnNode();

	std::vector<OrientationOnNode> allPossibleOrientations() const;
	void setOrientedEdges(std::vector<OrientedEdgeWrapper>& ori);

	void __check() const;
	bool isComplete() const;
};
