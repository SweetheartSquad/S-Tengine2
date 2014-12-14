#pragma once

#include <vector>
#include "NodeLoadable.h"

class NodeResource abstract : public virtual NodeLoadable{
protected:
	/** Whether safeDelete can be called or not */
	bool autoRelease;
public:

	explicit NodeResource(bool _autoRelease);
	/** Will cause this to delete itself if references is empty */
	/** Returns true if this was deleted and false if it wasn't */
	bool safeDelete();
	/** Simply calls dereference followed by safeDelete */
	void decrementAndDelete();
	/** Number of references to this */
	unsigned long int referenceCount;
	bool isAutoReleasing();
};