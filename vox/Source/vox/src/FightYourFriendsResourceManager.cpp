#pragma once

#include "FightYourFriendsResourceManager.h"
#include <TextureSampler.h>
#include <SoundManager.h>

TextureSampler * FightYourFriendsResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/FYFSplash.png", 1024, 1024, true, true), 1024, 1024);
Texture * FightYourFriendsResourceManager::arena1	= new Texture("../assets/hurly-burly/fyfArenas/arena1.png", 1024, 1024, true, true);
Texture * FightYourFriendsResourceManager::arena2Bg	= new Texture("../assets/hurly-burly/fyfArenas/arena2-bg.png", 1024, 1024, true, true);
TextureSampler * FightYourFriendsResourceManager::arena2Fg	= new TextureSampler("../assets/hurly-burly/fyfArenas/", "arena2-fg.png.def");

void FightYourFriendsResourceManager::init(){
	resources.push_back(splashMessage);
	resources.push_back(arena1);
	resources.push_back(arena2Bg);
	resources.push_back(arena2Fg);
}