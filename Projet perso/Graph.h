#pragma once

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
