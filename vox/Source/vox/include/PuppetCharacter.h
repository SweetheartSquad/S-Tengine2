#pragma once

#include <Box2D/Box2D.h>

#include "MeshEntity.h"

class Box2DWorld;

class PuppetCharacter : public MeshEntity{
public:

	bool ai;
	bool reactiveFeet;
	bool reactiveBody;
	static int16 gGroupIndex;
	int16 groupIndex;
	int16 categoryBits; // Box2D entity category
	int16 maskBits;		// Box2D collides only with
	float componentScale;

	Box2DWorld * world;

	PuppetCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, bool _ai = true);
	~PuppetCharacter();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;
};