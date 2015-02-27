#pragma once

#include <Box2D/Box2D.h>

#include "Box2DSuperSprite.h"

class Box2DSprite;
class Box2DWorld;

class PuppetCharacter : public Box2DSuperSprite {
public:
	bool ai;
	bool canJump;

	float targetRoll;

	Box2DSprite * head;
	Box2DSprite * torso;
	Box2DSprite * armLeft;
	Box2DSprite * armRight;
	Box2DSprite * handLeft;
	Box2DSprite * handRight;

	Box2DSprite * face;
	Box2DSprite * headgear;

	PuppetCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, bool _ai = true);
	~PuppetCharacter();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;
	//Called when the controller is thrust forward
	void action();
};