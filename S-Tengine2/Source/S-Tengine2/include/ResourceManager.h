#pragma once

#include "node/NodeLoadable.h"
#include "node/NodeResource.h"

#include <node\Node.h>
#include <vector>

class ResourceManager abstract : public Node{
public:	
	static std::vector<NodeResource *> resources;
	static std::vector<ResourceManager *> subManagers;
	static void load();
	static void unload();
	static void destruct();
};