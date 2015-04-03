#pragma once

#include <Box2D/Box2D.h>

#include "StructureInteractable.h"

class Box2DSprite;
class Box2DWorld;

class Boulder;
class PuppetCharacter;

class Catapult : public StructureInteractable{
public:

	bool fireBoulder;
	float cooldownCnt;

	Boulder * boulder;
	b2WeldJoint * boulderJoint;

	Box2DSprite * arm;
	Box2DSprite * base;

	Catapult(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void unload() override;
	void load() override;
	
	void evaluateState() override;
	void trigger(PuppetCharacter * _playerWhoTriggered) override;
	void prepare() override;
	void attemptInteract() override;
	void actuallyInteract() override;
};