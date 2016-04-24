#include "Edge.h"
#include "Node.h"


#include <exception>

Edge::Edge(Node* n1, Node* n2) :
Node1(n1),
Node2(n2),
fixed(false)
{
	if (n1 == NULL || n2 == NULL)
		throw std::exception();

}


Edge::~Edge()
{
}




bool Edge::isLoop() const
{
	return Node1 == Node2;
}

bool Edge::isFixed() const{
	return fixed;
}

void Edge::reset() {
	fixed = false;
}

void Edge::fixe() {
	fixed = true;
}

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
	os << e.Node1->getInternalNumber() << " -> " << e.Node2->getInternalNumber();
	return os;
}
