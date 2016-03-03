#include "node/NodeResource.h"

NodeResource::NodeResource(bool _autoRelease) :
	autoRelease(_autoRelease),
	referenceCount(0)
{
}

NodeResource::~NodeResource(){
}

bool NodeResource::safeDelete(){
	if((referenceCount == 0 || referenceCount == -1) && autoRelease){
		delete this;
		return true;
	}
	return false;
}

bool NodeResource::decrementAndDelete(){
	--referenceCount;
	return safeDelete();
}

bool NodeResource::isAutoReleasing(){
	return autoRelease;
}