#pragma once

#include <vector>

#include "Edge.h"

using OrientationOnNode = std::vector<Edge>;

class Node {
	friend class Graph;
	friend class Edge;
	friend class Node;

public:
	Node(int i);
	~Node();

	int degree() const;
	void addAdjacentEdge(Edge* ed);
	bool isEulerian() const;
	bool isWeaklyEulerian() const;
	int getInternalNumber() const;

	void __check_validity();

	std::vector<OrientationOnNode> allPossibleOrientations() const;
	void setOrientedEdges(const OrientationOnNode& ori);
	OrientationOnNode saveOrientation() const;

	bool isComplete() const;

private:
	std::vector<Edge*> edges;
	int internalNumber;
};



