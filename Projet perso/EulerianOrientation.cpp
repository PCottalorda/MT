#include "EulerianOrientation.h"

#include "Node.h"

#include <string>

std::string EulerianOrientation::generateGraphVizString() const {
	std::string graphVizString("");
	for each (const Edge& e in edges) {
		graphVizString += "\t\"" + std::to_string(internalNumber) + "_" + std::to_string(e.getHead()->getInternalNumber())
			+ "\" -> \"" + std::to_string(internalNumber) + "_" + std::to_string(e.getTail()->getInternalNumber()) + "\"\n";
	}
	return graphVizString;
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
