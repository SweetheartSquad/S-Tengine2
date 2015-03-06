#pragma once

#include <Box2D/Box2D.h>

#include "Structure.h"

class Box2DSprite;
class Box2DWorld;

class Castle : public Structure{
public:

	unsigned int health;
	unsigned int damage;

	Box2DSprite * base;

	Castle(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Castle();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;
};