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


#include "Graph.h"


#include <algorithm>
#include <string>
#include <functional>
#include <cassert>

Graph::Graph(unsigned nbVertices) :
	numberVertices(nbVertices),
	internalNodeCounter(0), internalEdgeCounter(0) {
	for (unsigned int i = 0; i < numberVertices; i++) {
		nodes.push_back(new Node(i));
	}
}

Graph::~Graph() {
}

void Graph::addEdge(int i, int j, const HomologieValue& value) {
	// An exception will be send if out of range
	// Ameliorates this
	Node* node1 = nodes[i];
	Node* node2 = nodes[j];

	edges.push_back(new Edge(node1, node2, internalEdgeCounter, value));
	internalEdgeCounter++;
	Edge* newNode = edges.back();

	node1->addAdjacentEdge(newNode);

	if (!newNode->isLoop()) {
		node2->addAdjacentEdge(newNode);
	}

	// TODO: OPTIMIZE!
	std::sort(edges.begin(), edges.end());
	assert(std::is_sorted(edges.begin(),edges.end()));
	assert(std::none_of(edges.begin(), edges.end(), std::mem_fn(&Edge::isLocked)));
}

std::vector<Edge*>& Graph::getEdges() {
	return edges;
}

std::vector<Node*>& Graph::getNodes() {
	return nodes;
}

bool Graph::isConsistent() const {

	for (auto it = edges.begin(); it != edges.end(); ++it) {
		if ((__fast_ptr_nodes.find((*it)->getDestination()) == __fast_ptr_nodes.end())
			|| ((__fast_ptr_nodes.find((*it)->getOrigin()) == __fast_ptr_nodes.end()))) { // One of the end of the edges does not comes from nodes
			return false;
		}
	}
	return true;
}

bool Graph::__internal_correct_node(const Node* node) {
	return !(__fast_ptr_nodes.find(node) == __fast_ptr_nodes.end());
}

bool Graph::__internal_correct_edge(const Edge& ed) {
	return __internal_correct_node(ed.getOrigin()) && __internal_correct_node(ed.getDestination());
}

bool Graph::isEulerian() {
	return std::all_of(nodes.begin(), nodes.end(), [](const Node* n) {
		                   return n->isEulerian();
	                   });
}

std::vector<EulerianOrientation> Graph::generateAllEulerianOrientations() {

	std::vector<EulerianOrientation> eulOri;
	assert(eulOri.empty());

	//Check the possibility
	__kernel_generateEulerian(0, eulOri);
	assert(!eulOri.empty());

	return eulOri;
}

void Graph::__kernel_generateEulerian(unsigned i, std::vector<EulerianOrientation>& eulOri) {
	//std::cout << "kernelGenerator step: " << i << std::endl;
	static unsigned int counter = 1;

	if (i >= nodes.size()) {
		//std::cout << "\tNew eulerian orientation found [" << counter << "]" << std::endl;
		counter++;
		eulOri.push_back(EulerianOrientation(this, internalNodeCounter));
		internalNodeCounter++;
	} else {
		Node* refNode = nodes[i];
		if (refNode->isComplete()) {
			if (refNode->isEulerian()) {
				__kernel_generateEulerian(i + 1, eulOri);
			} else {
				// The current restricted orientation is not eulerian
				// and cannot be extended into an eulerian orientation.
			}
		} else {
			std::vector<OrientationOnNode> ori(refNode->allPossibleOrientations());
			OrientationOnNode saveOri(refNode->saveOrientation());
			for each (const OrientationOnNode& o in ori) {
				refNode->setOrientedEdges(o);
				assert(refNode->isComplete());
				assert(refNode->isEulerian());
				__kernel_generateEulerian(i + 1, eulOri);
			}
			refNode->setOrientedEdges(saveOri);
		}
	}
}

std::string Graph::generateGraphVizString() const {
	std::string graphVizString("digraph G {\n");
	for each (const Edge* e in edges) {
		graphVizString += "\t\"" + std::to_string(e->getOrigin()->getInternalNumber())
			+ "\" -> \"" + std::to_string(e->getDestination()->getInternalNumber())
			+ "\" [arrowhead=none]\n";
	}
	graphVizString += "}\n\n";
	return graphVizString;
}

void Graph::__reset_ptr_nodes() {
	__fast_ptr_nodes.clear();
	for (size_t i = 0; i < nodes.size(); i++) {
		__fast_ptr_nodes.insert(nodes[i]);
	}
}

void Graph::reset_nodes_internalNumbers() {
	for (size_t i = 0; i < nodes.size(); i++) {
		nodes[i]->internalNumber = i;
	}
}
