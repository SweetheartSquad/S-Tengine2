#pragma once

#include <Joystick.h>
#include <Keyboard.h>

// maps keyboard to standard joystick controls
class JoystickVirtual : public Joystick{
public:
	Keyboard * keyboard;
	JoystickVirtual(int _id);

	virtual void update(Step * _step) override;
};