#pragma once

#include <node\NodeUpdatable.h>

#include <GLFW\glfw3.h>
#include <Joystick.h>

class JoystickManager : public NodeUpdatable{
public:
	Joystick * joysticks[GLFW_JOYSTICK_LAST];

	JoystickManager();
	~JoystickManager();

	void update(Step * _step) override;
};