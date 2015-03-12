#pragma once

#include "node/NodeLoadable.h"
#include "node/NodeResource.h"

#include <vector>

class ResourceManager abstract : public NodeLoadable{
public:	
	static std::vector<NodeResource *> resources;
	static std::vector<ResourceManager *> subManagers;
	virtual void load() override;
	virtual void unload() override;
};