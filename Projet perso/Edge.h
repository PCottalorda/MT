#pragma once

#include <iostream>
#include <cstdint>

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
