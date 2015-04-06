#pragma once

#include <node/Node.h>
//#include <iostream>
//#include <cinder\app\AppBasic.h>

//signed long int Node::count = 0;
std::vector<Node *>Node::nodes;

Node::~Node(){
	//--count;

	for(signed long int j = nodes.size()-1; j >= 0; --j){
		if(nodes.at(j) == this){
			nodes.erase(nodes.begin() + j);
		}
	}
	//std::cout << "~Node()\t" << this << "\t" << nodes.size() << std::endl;
}
Node::Node(){
	//++count;
	nodes.push_back(this);
	//std::cout << "Node()\t" << this << "\t" << nodes.size() << std::endl;
}