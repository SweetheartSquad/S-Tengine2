#pragma once

#include "Node.h"
#include <cinder\app\AppBasic.h>

Node::~Node(){
	ci::app::console() << "~Node()\t" << this << std::endl;
}