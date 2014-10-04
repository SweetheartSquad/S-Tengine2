#pragma once

#include <vector>

class ReferenceManager
{
private:
	//Objects which hold a reference to this
	std::vector<void*> references;
	//Whether safeDelete can be called or not
	bool autoRelease;
public:
	explicit ReferenceManager(bool _autoRelease);
	virtual ~ReferenceManager();
	//Removes a reference from references
	void dereference(void* _reference);
	//Adds _reference to references
	void attachReference(void* _reference);
	//Will cause this to delete itself if references is empty
	void safeDelete();
	//Simply calls dereference followed by safeDelete
	void dereferenceAndDelete(void* _reference);
	bool isAutoReleasing();
};