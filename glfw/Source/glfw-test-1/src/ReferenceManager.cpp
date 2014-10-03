#include "ReferenceManager.h"

void ReferenceManager::dereference(void* _reference){
	for(int i = 0; i < references.size(); i++){
		if(references.at(i) == _reference){
			references.erase(references.begin() + i);
			break;
		}
	}
}

void ReferenceManager::safeDelete(){
	if(references.size() == 0){
		delete this;
	}
}

void ReferenceManager::dereferenceAndDelete(void* _reference){
	dereference(_reference);
	safeDelete();
}

void ReferenceManager::attachReference(void* _reference){
	references.push_back(_reference);
}