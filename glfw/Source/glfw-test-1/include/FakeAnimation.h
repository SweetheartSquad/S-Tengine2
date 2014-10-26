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
	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights);
	/** Doesn't do anything by default */
	virtual void update();

};