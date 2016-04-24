#pragma once

#include <iostream>
#include <cstdint>

class Node;

enum Orientation {
	STANDARD,
	REVERSE
};

class Edge {
	friend class Graph;
	friend class Node;
	friend class Edge;

public:
	Edge(const Edge& other);
	Edge(Edge&& other);
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

	Edge& operator=(const Edge& other);
	Edge& operator=(Edge&& other);
	friend bool operator==(const Edge& lhs, const Edge& rhs);
	friend bool operator!=(const Edge& lhs, const Edge& rhs);

	bool isLoop() const;
	bool isAdjacentTo(const Node* node) const;

	uint64_t getId() const;

	friend bool operator<(const Edge& e1, const Edge& e2) {
		return e1.id < e2.id;
	}

	friend std::ostream& operator<<(std::ostream& os, const Edge& e);

protected:
	Node* Node1;
	Node* Node2;
	Orientation ori;
	bool locked;
	uint64_t id;
	Edge(Node* n1, Node* n2, uint64_t id);
};

std::ostream& operator<<(std::ostream& os, const Edge& e);
