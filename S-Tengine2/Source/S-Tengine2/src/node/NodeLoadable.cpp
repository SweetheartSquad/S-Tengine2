#pragma once

#include "node/NodeLoadable.h"

NodeLoadable::NodeLoadable():
	Node(),
	loaded(false)
{
}

NodeLoadable::~NodeLoadable(){
}

void NodeLoadable::load(){
	loaded = true;
}
void NodeLoadable::unload(){
	loaded = false;
}