#pragma once

#include <node/NodeLoadable.h>

#include <vector>
#include <ostream>

class NodeResource abstract : public virtual NodeLoadable{
private:
	/** Number of references to this */
	unsigned long int referenceCount;
	/** Whether safeDelete can be called or not */
	bool autoRelease;
public:

	explicit NodeResource(bool _autoRelease);
	virtual ~NodeResource();

	/** Will cause this to delete itself if references is empty */
	/** Returns true if this was deleted and false if it wasn't */
	bool safeDelete();
	/** Simply calls dereference followed by safeDelete */
	/** Returns true if this was deleted and false if it wasn't */
	bool decrementAndDelete();

	// increments the reference count and returns the current total
	unsigned long int incrementReferenceCount();

	bool isAutoReleasing();


	friend std::ostream& operator<<(std::ostream& os, const NodeResource& obj){
		return os
			<< static_cast<const NodeLoadable&>(obj)
			<< " autoRelease: " << obj.autoRelease
			<< " referenceCount: " << obj.referenceCount;
	}
};