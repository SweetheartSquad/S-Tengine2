#pragma once

#include <vector>
#include <ostream>

/** Abstract node */
class Node abstract{
public:
#ifdef _DEBUG
	static bool nodeCounting;
	static std::vector<Node *> nodes;
#endif

	std::string name;

	Node();
	virtual ~Node();

	friend std::ostream& operator<<(std::ostream& os, const Node& obj){
		return os;
	}
};