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


#include <vector>

#include "Edge.h"

using OrientationOnNode = std::vector<Edge>;

class Node {
	friend class Graph;
	friend class Edge;
	friend class Node;

public:
	Node(int i);
	~Node();

	int degree() const;
	void addAdjacentEdge(Edge* ed);
	bool isEulerian() const;
	bool isWeaklyEulerian() const;
	int getInternalNumber() const;

	void __check_validity();

	std::vector<OrientationOnNode> allPossibleOrientations() const;
	void setOrientedEdges(const OrientationOnNode& ori);
	OrientationOnNode saveOrientation() const;

	bool isComplete() const;

private:
	std::vector<Edge*> edges;
	int internalNumber;
};
