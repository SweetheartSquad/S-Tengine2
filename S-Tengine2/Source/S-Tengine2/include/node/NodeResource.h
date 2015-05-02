#pragma once

#include <vector>
#include "node/NodeLoadable.h"
#include <ostream>

class NodeResource abstract : public virtual NodeLoadable{
public:
	/** Whether safeDelete can be called or not */
	bool autoRelease;

	explicit NodeResource(bool _autoRelease);
	/** Will cause this to delete itself if references is empty */
	/** Returns true if this was deleted and false if it wasn't */
	bool safeDelete();
	/** Simply calls dereference followed by safeDelete */
	/** Returns true if this was deleted and false if it wasn't */
	bool decrementAndDelete();

	/** Number of references to this */
	unsigned long int referenceCount;
	bool isAutoReleasing();


	friend std::ostream& operator<<(std::ostream& os, const NodeResource& obj){
		return os
			<< static_cast<const NodeLoadable&>(obj)
			<< " autoRelease: " << obj.autoRelease
			<< " referenceCount: " << obj.referenceCount;
	}
};