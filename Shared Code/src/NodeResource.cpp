#include "NodeResource.h"

NodeResource::NodeResource(bool _autoRelease) :
	autoRelease(_autoRelease)
{
}

bool NodeResource::safeDelete(){
	if(referenceCount == 0 && autoRelease){
		delete this;
		return true;
	}
	return false;
}

void NodeResource::decrementAndDelete(){
	--referenceCount;
	safeDelete();
}

bool NodeResource::isAutoReleasing(){
	return autoRelease;
}