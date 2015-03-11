#pragma once

#include "ResourceManager.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "Box2DSuperSprite.h"

class TextureSampler;
class Texture;

class RaidTheCastleResourceManager : public ResourceManager{
public:

	static Texture * castle;

	static TextureSampler * catapultArm;
	static TextureSampler * catapultBody;
	static TextureSampler * boulder;

	RaidTheCastleResourceManager();
};