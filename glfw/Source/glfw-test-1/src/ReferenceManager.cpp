#include "ReferenceManager.h"

ReferenceManager::ReferenceManager(bool _autoRelease):
	autoRelease(_autoRelease){}

ReferenceManager::~ReferenceManager(){}

void ReferenceManager::dereference(void* _reference){
	for(int i = 0; i < references.size(); i++){
		if(references.at(i) == _reference || references.at(i) == nullptr){
			references.erase(references.begin() + i);
			break;
		}
	}
}

void ReferenceManager::safeDelete(){
	if(references.size() == 0 && autoRelease){
		delete this;
	}
}

void ReferenceManager::dereferenceAndDelete(void* _reference){
	dereference(_reference);
	safeDelete();
}

bool ReferenceManager::isAutoReleasing(){
	return autoRelease;
}

void ReferenceManager::attachReference(void* _reference){
	references.push_back(_reference);
}