#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "Box2DSuperSprite.h"

class TextureSampler;
class Texture;

class SlayTheDragonResourceManager abstract : public ResourceManager{
public:
	static void init();
};