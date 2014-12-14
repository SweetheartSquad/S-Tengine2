#pragma once

#include "node/Node.h"

class NodeLoadable abstract : public virtual Node{
public:
	NodeLoadable();
	virtual ~NodeLoadable();
	
	/** Load state */
	bool loaded;

	/** Override this function with stuff to load */
	virtual void load() = 0;
	/** Override this function with stuff to unload */
	virtual void unload() = 0;
};