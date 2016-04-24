#pragma once

#include <vector>

#include "Edge.h"
#include "Graph.h"

class EulerianOrientation {
	friend Graph;

public:
	EulerianOrientation(Graph* G, int tag);
	~EulerianOrientation();;

	std::string generateGraphVizString() const;

private:


	Graph* BaseGraph;
	std::vector<Edge> edges;

	int internalNumber;
};
