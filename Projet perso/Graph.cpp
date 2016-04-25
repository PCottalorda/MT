#include "Graph.h"

#include "EulerianOrientation.h"

#include <algorithm>
#include <exception>
#include <string>
#include <cstdint>


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

	//Check the possibility
	__kernel_generateEulerian(0, eulOri);

	std::cerr << "digraph G {\n";
	for each (const EulerianOrientation& eO in eulOri) {
		std::cerr << eO.generateGraphVizString() << std::endl;
	}
	std::cerr << "}" << std::endl;

	return eulOri;
}

void Graph::__kernel_generateEulerian(unsigned i, std::vector<EulerianOrientation> eulOri) {
	if (i >= nodes.size()) {
		// TODO:: generate Eulerian Orientation
		eulOri.push_back(EulerianOrientation(this, internalNodeCounter));
		internalNodeCounter++;
	}
	else {
		Node* refNode = nodes[i];
		if (refNode->isComplete()) {
			if (refNode->isEulerian()) {
				__kernel_generateEulerian(i + 1, eulOri);
			}
			else {
				// The current restricted orientation is not eulerian
				// and cannot be extended
			}
		}
		else {
			std::vector<OrientationOnNode> ori(refNode->allPossibleOrientations());
			OrientationOnNode saveOri(refNode->saveOrientation());
			for each (const OrientationOnNode &o in ori) {
				refNode->setOrientedEdges(o);
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
