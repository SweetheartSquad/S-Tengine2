#pragma once

#include <Box2D/Box2D.h>

#include "Structure.h"

class Sprite;
class Box2DSprite;
class Box2DWorld;

class Fortification : public Structure{
public:

	float health;
	float damage;

	Box2DSprite * base;
	Sprite * background;
	Box2DSprite * roof;

	Fortification(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Fortification();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	enum state_t{
		kNORMAL,
		kDAMAGED,
		kDEAD
	} state;
};