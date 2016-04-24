#pragma once

#include <iostream>

class Node;

class Edge
{
	friend class Graph;
	friend class Node;
	friend class OrientedEdgeWrapper;

public:
	Edge(Node* n1, Node* n2);


	Edge(const Edge& other)
		: Node1(other.Node1),
		  Node2(other.Node2)
	{
	}

	Edge(Edge&& other)
		: Node1(other.Node1),
		  Node2(other.Node2)
	{
	}

	Edge& operator=(const Edge& other)
	{
		if (this == &other)
			return *this;
		Node1 = other.Node1;
		Node2 = other.Node2;
		return *this;
	}

	Edge& operator=(Edge&& other)
	{
		if (this == &other)
			return *this;
		Node1 = other.Node1;
		Node2 = other.Node2;
		return *this;
	}

	~Edge();

	bool isLoop() const;

	bool isAdjacentTo(const Node* node) const
	{
		return (node == Node1) || (node == Node2);
	}

	friend std::ostream& operator<<(std::ostream& os, const Edge& e);

protected:
	Node* Node1;
	Node* Node2;
};

std::ostream& operator<<(std::ostream& os, const Edge& e);

enum Orientation
{
	STANDARD,
	REVERSE
};

class OrientedEdgeWrapper
{

public:
	Orientation ori;
	Edge* base;
	bool locked;

	bool isLocked() const
	{
		return locked;
	}

	void lock()
	{
		locked = true;
	}

	void unLock()
	{
		locked = false;
	}

	

	const Node* getOrigin() const 
	{
		if (ori == STANDARD)
		{
			return base->Node1;
		} else
		{
			return base->Node2;
		}
	}

	const Node* getDestination() const 
	{
		if (ori == STANDARD)
		{
			return base->Node2;
		} else
		{
			return base->Node1;
		}
	}


	OrientedEdgeWrapper(const OrientedEdgeWrapper& other)
		: ori(other.ori),
		  base(other.base)
	{
	}

	OrientedEdgeWrapper(OrientedEdgeWrapper&& other)
		: ori(other.ori),
		  base(other.base)
	{
	}

	OrientedEdgeWrapper& operator=(const OrientedEdgeWrapper& other)
	{
		if (this == &other)
			return *this;
		ori = other.ori;
		base = other.base;
		return *this;
	}

	OrientedEdgeWrapper& operator=(OrientedEdgeWrapper&& other)
	{
		if (this == &other)
			return *this;
		ori = other.ori;
		base = other.base;
		return *this;
	}

	friend bool operator==(const OrientedEdgeWrapper& lhs, const OrientedEdgeWrapper& rhs)
	{
		return (lhs.getOrigin() == rhs.getOrigin()) && (lhs.getDestination() == rhs.getDestination());
	}

	friend bool operator!=(const OrientedEdgeWrapper& lhs, const OrientedEdgeWrapper& rhs)
	{
		return !(lhs == rhs);
	}


	Orientation getOrientation() const
	{
		return ori;
	}

	void setOrientation(Orientation ori)
	{
		this->ori = ori;
	}

	void reverseOrientation()
	{
		if (ori == STANDARD)
		{
			ori = REVERSE;
		} else
		{
			ori = STANDARD;
		}
	}

	void setOrigin(const Node* node)
	{
		if (base->isAdjacentTo(node))
		{
			if (getOrigin() != node)
			{
				reverseOrientation();
			}
		} else
		{
			throw std::exception();
		}
	}

	void setDestination(const Node* node)
	{
		if (base->isAdjacentTo(node))
		{
			if (getOrigin() != node)
			{
				reverseOrientation();
			}
		}
		else
		{
			throw std::exception();
		}
	}
};