#pragma once

#include <vector>

#include "MeshEntity.h"

class Shader;
class Transform;

class FakeAnimation : public MeshEntity {
public:
	unsigned long int frame;
	unsigned long int delay;
	unsigned long int delayCount;

	FakeAnimation(Transform * _transform, Shader * _shader);
	~FakeAnimation();

	void pushFrame(MeshEntity * _frame);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	/** Doesn't do anything by default */
	virtual void update(Step * _step) override;

};