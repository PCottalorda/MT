#pragma once

#include "Node.h"
#include "Edge.h"

#include <vector>
#include <unordered_set>
#include "EulerianOrientation.h"

class Graph {
public:
	Graph(unsigned int nbVertices);
	~Graph();


	void addEdge(int i, int j, const HomologieValue& value = HomologieValue(1));
	std::vector<Edge*>& getEdges();
	std::vector<Node*>& getNodes();

	bool isConsistent() const;
	bool __internal_correct_node(const Node* node);
	bool __internal_correct_edge(const Edge& ed);

	bool isEulerian();

	std::vector<EulerianOrientation> generateAllEulerianOrientations();
	void __kernel_generateEulerian(unsigned int i, std::vector<EulerianOrientation>& eulOri);
	std::string generateGraphVizString() const;


private:
	std::unordered_set<const Node*> __fast_ptr_nodes;
	unsigned int numberVertices;

	int internalNodeCounter;
	int internalEdgeCounter;
	std::vector<Node *> nodes;
	std::vector<Edge *> edges;

	void __reset_ptr_nodes();
	void reset_nodes_internalNumbers();
};
