#pragma once

#include "node/NodeLoadable.h"

NodeLoadable::NodeLoadable():
	Node(),
	loaded(false)
{
	nodeType |= NodeType::kNODE_LOADABLE;
}

NodeLoadable::~NodeLoadable(){
}

void NodeLoadable::load(){
	loaded = true;
}
void NodeLoadable::unload(){
	loaded = false;
}