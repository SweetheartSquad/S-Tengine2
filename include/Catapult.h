#pragma once

#include <Box2D/Box2D.h>

#include "Structure.h"

class CharacterComponent;
class Box2DWorld;

class Catapult : public Structure{
public:

	float targetRoll;
	
	CharacterComponent * arm;
	CharacterComponent * base;

	std::vector<CharacterComponent ** > components;

	Catapult(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1);
	~Catapult();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

};