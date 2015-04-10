#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"

class SoundManager;
class TextureSampler;
class Texture;

class FightYourFriendsResourceManager abstract : public ResourceManager{
public:
	static TextureSampler * splashMessage;

	static void init();
};