#pragma once 

#include <StructureInteractable.h>

class SpriteSheetAnimation;

class StructureGoldPile : public StructureInteractable{
public:

	int totalGold;
	int goldTaken;

	SpriteSheetAnimation * spriteSheet;

	explicit StructureGoldPile(Box2DWorld* _world);
	~StructureGoldPile();

	void actuallyInteract() override;
	void evaluateState() override;
};