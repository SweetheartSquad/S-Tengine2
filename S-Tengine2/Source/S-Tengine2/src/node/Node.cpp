#pragma once

#include <node/Node.h>

#ifdef _DEBUG
std::vector<Node *>Node::nodes;
bool Node::nodeCounting = false;
#endif

Node::~Node(){
#ifdef _DEBUG
	if(nodeCounting){
		for(signed long int j = nodes.size()-1; j >= 0; --j){
			if(nodes.at(j) == this){
				nodes.erase(nodes.begin() + j);
			}
		}
	}
#endif
}
Node::Node(){
#ifdef _DEBUG
	name = "UNNAMED";
	if(nodeCounting){
		nodes.push_back(this);
	}
#endif
}