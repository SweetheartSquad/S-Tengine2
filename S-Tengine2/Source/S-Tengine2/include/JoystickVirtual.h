#pragma once

#include <Joystick.h>
#include <Keyboard.h>

// maps keyboard to standard joystick controls
class JoystickVirtual : public Joystick{
public:
	JoystickVirtual(int _id);
	virtual void update(Step * _step) override;

protected:
	Keyboard * keyboard;
	
	int axisLeftDown,
		axisLeftUp,
		axisLeftLeft,
		axisLeftRight,
		
		axisRightDown,
		axisRightUp,
		axisRightLeft,
		axisRightRight,
		
		axisTriggerLeft,
		axisTriggerRight;
	
	// sets the button codes to correspond to generic keyboard controls
	void initVirtual();

	void handleButton(int _glfwKeyCode);
};