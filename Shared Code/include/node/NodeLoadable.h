#pragma once

#include "node/Node.h"

class NodeLoadable abstract : public virtual Node{
public:
	NodeLoadable();
	virtual ~NodeLoadable();
	
	/** Load state */
	bool loaded;

	/** Override this function with stuff to load; remember to call it in the derived class */
	virtual void load();
	/** Override this function with stuff to unload; remember to call it in the derived class */
	virtual void unload();
};