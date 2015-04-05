#pragma once

#include "node/Node.h"
//#include <cinder\app\AppBasic.h>

unsigned long int Node::count = 0;
std::vector<Node *>Node::nodes;

Node::~Node(){
	--count;

	for(signed long int j = nodes.size()-1; j >= 0; --j){
		if(nodes.at(j) == this){
			nodes.erase(nodes.begin() + j);
		}
	}
	//ci::app::console() << "~Node()\t" << this << "\t" << count << std::endl;
}
Node::Node(){
	++count;
	nodes.push_back(this);
	//ci::app::console() << "Node()\t" << this << "\t" << count << std::endl;
}