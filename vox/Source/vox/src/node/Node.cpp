#pragma once

#include "node/Node.h"
//#include <cinder\app\AppBasic.h>

unsigned long int Node::count = 0;

Node::~Node(){
	--count;
	//ci::app::console() << "~Node()\t" << this << "\t" << count << std::endl;
}
Node::Node(){
	++count;
	//ci::app::console() << "Node()\t" << this << "\t" << count << std::endl;
}