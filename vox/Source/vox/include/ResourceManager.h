#pragma once

#include "node/NodeLoadable.h"
#include "node/NodeResource.h"

#include <vector>

class ResourceManager abstract : public NodeLoadable{
public:	
	std::vector<NodeResource *> resources;
	std::vector<ResourceManager *> subManagers;
	void load() override;
	void unload() override;
};