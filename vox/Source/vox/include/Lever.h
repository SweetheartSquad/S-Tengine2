#pragma once

#include <StructureInteractable.h>

class Lever : public StructureInteractable{
public:
	bool pullingLever;
	float cooldownCnt;
	
	Box2DSprite * base;
	Box2DSprite * handle;

	Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex);
	~Lever();
	
	void evaluateState() override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void unload() override;
	void load() override;
};