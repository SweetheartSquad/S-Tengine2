#pragma once 

#include <Structure.h>

class StructureGoldPile : public Structure{
public:

	int totalGold;
	int goldTaken;

	explicit StructureGoldPile(Box2DWorld* _world);
	~StructureGoldPile();

	void loseGold();
};