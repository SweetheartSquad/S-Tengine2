#pragma once

#include "FightYourFriendsResourceManager.h"
#include <TextureSampler.h>
#include <SoundManager.h>

TextureSampler * FightYourFriendsResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/FYFSplash.png", 1024, 1024, true, true), 1024, 1024);
Texture * FightYourFriendsResourceManager::arena	= new Texture("../assets/hurly-burly/fyfArenas/arena1.png", 1024, 1024, true, true);

void FightYourFriendsResourceManager::init(){
	resources.push_back(splashMessage);
	resources.push_back(arena);
}