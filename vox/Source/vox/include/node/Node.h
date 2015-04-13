#pragma once

#include <vector>

/** Abstract node */
class Node abstract{
public:
#ifdef _DEBUG
	static std::vector<Node *> nodes;
#endif

	Node();
	virtual ~Node();
};