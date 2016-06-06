

/*****************************************************************************
*                                                                            *
*  Copyright 2016 Paul Cottalorda                                            *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
******************************************************************************/


#include "EulerianOrientation.h"

#include "Node.h"
#include "Graph.h"

#include <string>
#include <cassert>

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
