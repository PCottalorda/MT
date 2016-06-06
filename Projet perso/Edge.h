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



#include <cstdint>
#include <iostream>

#include "HomologieValue.h"

class Node;

enum Orientation {
	STANDARD,
	REVERSE
};

class Edge {
	friend class Graph;
	friend class Node;
	friend class EulerianOrientation;

public:
	Edge(const Edge& other);
	Edge(Edge&& other);

	Edge& operator=(const Edge& other);
	Edge& operator=(Edge&& other);

	~Edge();


	bool isLocked() const;
	void lock();
	void unLock();

	void reverseOrientation();

	void setOrigin(const Node* node);
	void setDestination(const Node* node);

	Node* getOrigin();
	Node* getDestination();

	const Node* getOrigin() const;
	const Node* getDestination() const;

	friend bool operator==(const Edge& lhs, const Edge& rhs);
	friend bool operator!=(const Edge& lhs, const Edge& rhs);

	bool isLoop() const;
	bool isAdjacentTo(const Node* node) const;

	uint64_t getId() const;

	friend bool operator<(const Edge& e1, const Edge& e2);
	friend std::ostream& operator<<(std::ostream& os, const Edge& e);

	HomologieValue getHomologieValue() const {
		if (ori == STANDARD) {
			return value;
		} else {	// ori == REVERSE
			return -value;
		}
	}

	unsigned int dimension() const {
		return value.dimension();
	}

protected:
	Node* Node1;
	Node* Node2;
	Orientation ori;
	bool locked;
	uint64_t id;
	HomologieValue value;

	Edge(Node* node1, Node* node2, uint64_t id, HomologieValue value);
};

std::ostream& operator<<(std::ostream& os, const Edge& e);
