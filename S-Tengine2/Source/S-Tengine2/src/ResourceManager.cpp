#pragma once

#include "ResourceManager.h"
#include "scenario/Scenario.h"

std::vector<NodeResource *> ResourceManager::resources;
std::vector<ResourceManager *> ResourceManager::subManagers;

void ResourceManager::load(){
	//NodeLoadable::load();
	for(auto res : resources){
		res->load();
	}
	for(auto sub : subManagers){
		sub->load();
	}

	for(auto a : Scenario::defaultAssets){
		a->load();
	}
}

void ResourceManager::unload(){
	for(auto a : Scenario::defaultAssets){
		a->load();
	}
	//NodeLoadable::unload();
	for(auto res : resources){
		res->unload();
	}
	for(auto sub : subManagers){
		sub->unload();
	}
}


void ResourceManager::destruct(){
	while(resources.size() > 0){
		delete resources.back();
		resources.pop_back();
	}

	Scenario::destruct();
}