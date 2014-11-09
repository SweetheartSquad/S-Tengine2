#pragma once

#include <vector>

#include "Entity.h"

class Shader;
class Transform;

class FakeAnimation : public Entity {
public:
	unsigned long int frame;
	unsigned long int delay;
	unsigned long int delayCount;

	FakeAnimation(Transform * _transform, Shader * _shader, Entity * _parent);
	~FakeAnimation();

	void pushFrame(Entity * _frame);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void draw(MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	/** Doesn't do anything by default */
	virtual void update() override;

};