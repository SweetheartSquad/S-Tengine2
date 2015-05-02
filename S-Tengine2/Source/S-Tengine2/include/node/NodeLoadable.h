#pragma once

#include "node/Node.h"
#include <ostream>

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


	friend std::ostream& operator<<(std::ostream& os, const NodeLoadable& obj){
		return os
			<< static_cast<const Node&>(obj)
			<< " loaded: " << obj.loaded;
	}
};