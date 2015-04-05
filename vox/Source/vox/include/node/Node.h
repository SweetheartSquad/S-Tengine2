#pragma once

#include <vector>

/** Abstract node */
class Node abstract{
public:
	static unsigned long int count;
	static std::vector<Node *> nodes;

	Node();
	virtual ~Node();
};