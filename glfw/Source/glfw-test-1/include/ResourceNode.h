#pragma once

#include <vector>

class ResourceNode{
protected:
	/** Whether safeDelete can be called or not */
	bool autoRelease;
public:
	explicit ResourceNode(bool _autoRelease);
	virtual ~ResourceNode();
	/** Will cause this to delete itself if references is empty */
	/** Returns true if this was deleted and false if it wasn't */
	bool safeDelete();
	/** Simply calls dereference followed by safeDelete */
	void decrementAndDelete();
	/** Number of references to this */
	unsigned long int referenceCount;
	bool isAutoReleasing();

	/** Override this function with stuff to load */
	virtual void load() = 0;
	/** Override this function with stuff to unload */
	virtual void unload() = 0;
	/** Load state */
	bool loaded;
};