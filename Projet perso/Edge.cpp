
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


#include "Edge.h"
#include "Node.h"

#include <exception>

Edge::Edge(Node* n1, Node* n2, uint64_t id,HomologieValue value) :
	Node1(n1),
	Node2(n2),
	locked(false),
	ori(STANDARD),
	id(id),
	value(value) 
{
	if (n1 == NULL || n2 == NULL)
		throw std::exception();

}

bool Edge::isLoop() const {
	return Node1 == Node2;
}

bool Edge::isAdjacentTo(const Node* node) const {
	return (node == Node1) || (node == Node2);
}

uint64_t Edge::getId() const {
	return id;
}

bool operator<(const Edge& e1, const Edge& e2) {
	return e1.id < e2.id;
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
	os << e.getOrigin()->getInternalNumber() << " -> " << e.getDestination()->getInternalNumber();
	return os;
}

bool operator==(const Edge& lhs, const Edge& rhs) {
	return (lhs.getOrigin() == rhs.getOrigin()) && (lhs.getDestination() == rhs.getDestination()) && (lhs.id == rhs.id) && (lhs.value == rhs.value);
}

bool operator!=(const Edge& lhs, const Edge& rhs) {
	return !(lhs == rhs);
}

bool Edge::isLocked() const {
	return locked;
}

void Edge::lock() {
	locked = true;
}

void Edge::unLock() {
	locked = false;
}

const Node* Edge::getOrigin() const {
	if (ori == STANDARD) {
		return this->Node1;
	} else {
		return this->Node2;
	}
}

const Node* Edge::getDestination() const {
	if (ori == STANDARD) {
		return this->Node2;
	} else {
		return this->Node1;
	}
}

Node* Edge::getOrigin() {
	if (ori == STANDARD) {
		return this->Node1;
	} else {
		return this->Node2;
	}
}

Node* Edge::getDestination() {
	if (ori == STANDARD) {
		return this->Node2;
	} else {
		return this->Node1;
	}
}


Edge::Edge(const Edge& other): Node1(other.Node1),
                               Node2(other.Node2),
                               ori(other.ori),
                               locked(other.locked),
                               value(other.value),
                               id(other.id) {
}

Edge::Edge(Edge&& other): Node1(other.Node1),
                          Node2(other.Node2),
                          ori(other.ori),
                          locked(other.locked),
                          value(other.value),
                          id(other.id) {
}

Edge& Edge::operator=(const Edge& other) {
	if (this == &other)
		return *this;
	Node1 = other.Node1;
	Node2 = other.Node2;
	ori = other.ori;
	locked = other.locked;
	value = other.value;
	id = other.id;
	return *this;
}

Edge& Edge::operator=(Edge&& other) {
	if (this == &other)
		return *this;
	Node1 = other.Node1;
	Node2 = other.Node2;
	ori = other.ori;
	locked = other.locked;
	value = other.value;
	id = other.id;
	return *this;
}

Edge::~Edge() {
}


void Edge::reverseOrientation() {
	if (ori == STANDARD) {
		ori = REVERSE;
	} else {
		ori = STANDARD;
	}
}

void Edge::setOrigin(const Node* node) {
	if (this->isAdjacentTo(node)) {
		if (getOrigin() != node) {
			reverseOrientation();
		}
	} else {
		throw std::exception();
	}
}

void Edge::setDestination(const Node* node) {
	if (this->isAdjacentTo(node)) {
		if (getOrigin() != node) {
			reverseOrientation();
		}
	} else {
		throw std::exception();
	}
}
