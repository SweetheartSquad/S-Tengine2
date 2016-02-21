#pragma once

#include <node/NodeUpdatable.h>
#include <glm\glm.hpp>

#include <Keyboard.h>
#include <Mouse.h>
class Joystick;

class BulletController : public virtual NodeUpdatable{
protected:
	bool enabled;

	Keyboard & keyboard;
	Mouse & mouse;
	Joystick * joystick;

	// enables inputs
	virtual void enable();
	// disables inputs
	virtual void disable();

	// this must be overriden by the derived class
	// the values are interpreted differentely by derived classes, but typically
	// x = side-to-side movement, e.g. strafing, turning
	// z = front-to-back movement, e.g. legs walking, engine force
	// y = up-down movement, e.g. jump flag, flight
	// NOTE: You should also handle modifiers and additional flags (e.g. sprinting, braking) here
	// NOTE: This is only called if the controller is enabled. Code which must run regardless of activation state should be in handleInputs or update
	virtual glm::vec3 calculateInputs(Step * _step) = 0;
	virtual void handleInputs(Step * _step, glm::vec3 _inputs) = 0;

public:
	// when disabled, this vector is send to handleInputs() instead of the result of calculateInputs()
	glm::vec3 disabledInputs;
	
	// returns the value of the enabled bool
	bool isEnabled() const;

	// calls handleInputs(), using the result of calculateInputs() as an argument
	// if disabled, uses the disabledInputs instead
	virtual void update(Step * _step);

	BulletController();
};