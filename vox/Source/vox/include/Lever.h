#pragma once

#include <Structure.h>

class Lever :public Structure
{
public:
	bool ready;
	bool triggered;
	bool pullingLever;
	float cooldownCnt;
	
	Box2DSprite * base;
	Box2DSprite * handle;

	Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex);
	~Lever();
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	virtual void pullLever();

private:

};