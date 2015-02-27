#pragma once

#include <Box2D/Box2D.h>

#include "Structure.h"

class Box2DSprite;
class Box2DWorld;

class Catapult : public Structure{
public:

	bool ready;
	bool firing;
	bool fireBoulder;
	float cooldownCnt;

	Box2DSprite * arm;
	Box2DSprite * base;

	Catapult(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1);
	~Catapult();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void fireCatapult();
};