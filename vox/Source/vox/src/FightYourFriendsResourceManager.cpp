#pragma once

#include "FightYourFriendsResourceManager.h"
#include <TextureSampler.h>
#include <SoundManager.h>

TextureSampler * FightYourFriendsResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/FYFSplash.png", 1024, 1024, true, true), 1024, 1024);

void FightYourFriendsResourceManager::init(){
	resources.push_back(splashMessage);
}