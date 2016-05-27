#pragma once

#include <vector>

#include "Edge.h"
#include <cassert>

class Graph;

class EulerianOrientation {
	friend Graph;

public:
	EulerianOrientation(Graph* G, int tag);
	~EulerianOrientation();

	std::string generateGraphVizString() const;
	HomologieValue cycleValue() const;

private:


	Graph* BaseGraph;
	std::vector<Edge> edges;

	int internalNumber;
};
