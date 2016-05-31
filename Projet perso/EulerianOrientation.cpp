#include "EulerianOrientation.h"

#include "Node.h"
#include "Graph.h"

#include <string>

std::string EulerianOrientation::generateGraphVizString() const {
	std::string graphVizString("");
	for each (const Edge& e in edges) {
		graphVizString += "\t\"" + std::to_string(internalNumber) + "_" + std::to_string(e.getOrigin()->getInternalNumber())
			+ "\" -> \"" + std::to_string(internalNumber) + "_" + std::to_string(e.getDestination()->getInternalNumber()) + "\"\n";
	}
	return graphVizString;
}

HomologieValue EulerianOrientation::cycleValue() const {
	assert(!edges.empty());
	HomologieValue res(edges.front().dimension());
	for (const Edge& e : edges) {
		res += e.getHomologieValue();
	}
	return res;
}

EulerianOrientation::EulerianOrientation(Graph* G, int tag):
	internalNumber(tag),
	BaseGraph(G) {
	for each (const Edge* ed in G->getEdges()) {
		edges.push_back(Edge(*ed));
	}
}

EulerianOrientation::~EulerianOrientation() {
}
