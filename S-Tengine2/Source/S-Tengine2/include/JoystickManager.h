#pragma once

#include <node\NodeUpdatable.h>

#include <GLFW\glfw3.h>
#include <Joystick.h>

class JoystickManager : public NodeUpdatable{
public:
	Joystick * joysticks[GLFW_JOYSTICK_LAST];

	bool joystickPresent;
	float deadZone;

	JoystickManager(float _deadZone = 0.25f);
	~JoystickManager();

	virtual void update(Step * _step) override;

	virtual void reconnect();
};