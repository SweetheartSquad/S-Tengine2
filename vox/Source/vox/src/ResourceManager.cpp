#pragma once

#include "ResourceManager.h"

void ResourceManager::load(){
	NodeLoadable::load();
	for(auto res : resources){
		res->load();
	}
	for(auto sub : subManagers){
		sub->load();
	}
}

void ResourceManager::unload(){
	NodeLoadable::unload();
	for(auto res : resources){
		res->unload();
	}
	for(auto sub : subManagers){
		sub->unload();
	}
}