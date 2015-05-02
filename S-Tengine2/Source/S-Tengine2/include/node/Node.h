#pragma once

#include <vector>
#include <ostream>

/** Abstract node */
class Node abstract{
public:
#ifdef _DEBUG
	static std::vector<Node *> nodes;
#endif

	Node();
	virtual ~Node();

	friend std::ostream& operator<<(std::ostream& os, const Node& obj){
		return os;
	}
};