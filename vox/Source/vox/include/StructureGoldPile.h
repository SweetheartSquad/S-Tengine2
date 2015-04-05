#pragma once 

#include <Structure.h>

class SpriteSheetAnimation;

class StructureGoldPile : public Structure{
public:

	int totalGold;
	int goldTaken;

	SpriteSheetAnimation * spriteSheet;

	explicit StructureGoldPile(Box2DWorld* _world);
	~StructureGoldPile();

	void loseGold();
};