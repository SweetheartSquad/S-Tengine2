#include "ResourceNode.h"

ResourceNode::ResourceNode(bool _autoRelease) :
	autoRelease(_autoRelease),
	loaded(false)
{
}

ResourceNode::~ResourceNode(){
}

bool ResourceNode::safeDelete(){
	if(referenceCount == 0 && autoRelease){
		delete this;
		return true;
	}
	return false;
}

void ResourceNode::decrementAndDelete(){
	--referenceCount;
	safeDelete();
}

bool ResourceNode::isAutoReleasing(){
	return autoRelease;
}