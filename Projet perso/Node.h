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


using OrientationOnNode = std::vector <Edge>;

class Node {
	friend class Graph;
	friend class Edge;
	friend class Node;

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


	int getInternalNumber() const;
	std::vector<OrientationOnNode> allPossibleOrientations() const;
	void setOrientedEdges(OrientationOnNode& ori);
	std::vector<OrientationOnNode> saveOrientation() const {
		OrientationOnNode res(edges.size());
		for each(const Edge* ed in edges) {
			res.push_back(*ed);
		}
	}

	bool isComplete() const;

private:
	std::vector<Edge*> edges;

	bool marked;
	int internalNumber;
};



