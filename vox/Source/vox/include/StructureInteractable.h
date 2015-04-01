#pragma once

#include <Box2D/Box2D.h>

#include "Structure.h"

class Box2DSprite;
class Box2DWorld;
class PuppetCharacter;

class StructureInteractable : public Structure{
public:
	// whether the structure is ready to be prepared
	bool ready;
	// whether the structure is prepared to trigger
	bool prepared;
	// whether the structure will trigger in the next update
	bool triggering;
	// reference to the player who triggered the structure last
	PuppetCharacter * playerWhoTriggered;

	StructureInteractable(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	
	virtual void prepare();
	virtual void trigger(PuppetCharacter * _playerWhoTriggered);
	virtual void interact();

	virtual void update(Step * _step) override;
};