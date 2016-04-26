#include "Graph.h"


#include <algorithm>
#include <exception>
#include <string>
#include <cstdint>
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

void Graph::addEdge(int i, int j) {
	// An exception will be send if out of range
	// Ameliorates this
	Node* node1 = nodes[i];
	Node* node2 = nodes[j];

	edges.push_back(new Edge(node1, node2,internalEdgeCounter));
	internalEdgeCounter++;
	Edge* newNode = edges.back();

	node1->addAdjacentEdge(newNode);

	if (!newNode->isLoop()) {
		node2->addAdjacentEdge(newNode);
	}

	// TODO: OPTIMIZE!
	std::sort(edges.begin(),edges.end());
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
	return std::all_of(nodes.begin(), nodes.end(), [](const Node* n){return n->isEulerian(); });
}

std::vector<EulerianOrientation> Graph::generateAllEulerianOrientations() {

	std::vector<EulerianOrientation> eulOri;
	assert(eulOri.empty());

	//Check the possibility
	__kernel_generateEulerian(0, eulOri);
	assert(!eulOri.empty());

	std::cerr << "digraph G {\n";
	for each (const EulerianOrientation& eO in eulOri) {
		std::cerr << eO.generateGraphVizString() << std::endl;
	}
	std::cerr << "}" << std::endl;
	 
	return eulOri;
}

void Graph::__kernel_generateEulerian(unsigned i, std::vector<EulerianOrientation> eulOri) {
	std::cout << "kernelGenerator step: " << i << std::endl;
	if (i >= nodes.size()) {
		std::cout << "All nodes parcoured: New Eulerian Orientation!" << std::endl;
		// TODO:: generate Eulerian Orientation
		eulOri.push_back(EulerianOrientation(this, internalNodeCounter));
		internalNodeCounter++;
	}
	else {
		std::cout << "Node " << i << std::endl;
		Node* refNode = nodes[i];
		if (refNode->isComplete()) {
			std::cout << "\tComplete Node..." << std::endl;
			if (refNode->isEulerian()) {
				std::cout << "\t\tEulerian Node..." << std::endl;
				__kernel_generateEulerian(i + 1, eulOri);
			}
			else {
				std::cout << "\t\tNon Eulerian Node..." << std::endl;
				// The current restricted orientation is not eulerian
				// and cannot be extended
			}
		}
		else {
			std::cout << "\tIncomplete Node..." << std::endl;
			std::vector<OrientationOnNode> ori(refNode->allPossibleOrientations());
			std::cout << ori.size() << " possible eulerian orientations..." << std::endl;
			OrientationOnNode saveOri(refNode->saveOrientation());
			for each (const OrientationOnNode &o in ori) {
				// We check if the two orientations are compatible
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
