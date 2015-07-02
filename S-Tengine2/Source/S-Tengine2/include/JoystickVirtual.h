#pragma once

#include <Joystick.h>
#include <Keyboard.h>

// maps keyboard to standard joystick controls
class JoystickVirtual : public Joystick{
public:
	int faceButtonDown,  // A on xbox, X on playstation
		faceButtonUp,    // Y on xbox, triangle on playstation
		faceButtonLeft,  // X on xbox, square on playstation
		faceButtonRight, // B on xbox, circle on playstation
		
		axisLeftDown,
		axisLeftUp,
		axisLeftLeft,
		axisLeftRight,
		
		axisRightDown,
		axisRightUp,
		axisRightLeft,
		axisRightRight,
		
		axisTriggerLeft,
		axisTriggerRight,

		bumperLeft,
		bumperRight,

		centerButtonLeft,  // back on xbox, select on playstation
		centerButtonRight, // start
		
		dpadDown,
		dpadUp,
		dpadLeft,
		dpadRight;


	JoystickVirtual(int _id);
	virtual void update(Step * _step) override;

private:
	Keyboard * keyboard;
};