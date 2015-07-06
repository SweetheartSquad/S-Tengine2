#pragma once

#include <node\NodeUpdatable.h>

#include <iostream>
#include <map>

/************************************************************
*
* wrapper for joystick events and properties
*
*************************************************************/

class Joystick : public NodeUpdatable{
public:
	int faceButtonDown,  // A on xbox, X on playstation
		faceButtonUp,    // Y on xbox, triangle on playstation
		faceButtonLeft,  // X on xbox, square on playstation
		faceButtonRight, // B on xbox, circle on playstation
		
		axisButtonLeft,  // pressing the left analog stick
		axisButtonRight, // pressing the right analog stick

		bumperLeft,
		bumperRight,

		centerButtonLeft,   // back on xbox, select on playstation
		centerButtonCenter, // XBOX button, PS button (not reliable)
		centerButtonRight,  // start
		
		dpadDown,
		dpadUp,
		dpadLeft,
		dpadRight,
		
		axisLeftX, // left-right on the left analog stick
		axisLeftY, // up-down on the left analog stick
		axisTriggers, // corresponds to L2 and R2 (note that until GLFW updates the joystick input, we can only use the triggers separately)
		axisRightY, // up-down on the right analog stick
		axisRightX; // left-right on the right analog stick

	// when getAxis is called, any value less than deadZone will instead return zero
	float deadZone;

	// GLFW joystick id
	int id;

	// human-readable string intended to identify the make/model of a joystick
	std::string name;

	/**
	* Returns true if the given button is currently pressed down
	*
	* @param The joystick's button code
	* @return Whether the button is currently down or not
	*/
	bool buttonDown(int _code);

	/**
	* Returns true if the given button was released since the last call to update
	*
	* @param The joystick's button code
	* @return Whether the button was released since the last update
	*/
	bool buttonJustUp(int _code);

	/**
	* Returns true if the given button was pressed down since the last call to update
	*
	* @param The joystick's button code
	* @return Whether the button was pressed down since the last update
	*/
	bool buttonJustDown(int _code);

	// Returns the value of the given axis
	// If the absolute value is less than the deadZone, returns 0
	// If the axis doesn't exist, returns 0
	float getAxis(int _code);

	// If _axis was above _value last update
	// and is below _value now, returns true
	// otherwise, returns false
	bool axisJustBelow(int _axis, float _value);

	// If _axis was below _value last update
	// and is above _value now, returns true
	// otherwise, returns false
	bool axisJustAbove(int _axis, float _value);

	/**
	* Handles input from glfw
	*/
	virtual void update(Step * _step) override;

	/**
	* Inserts _code into the maps of justPressed and pressed buttons
	*
	* @param The button code
	*/
	void buttonDownListener(int _code);

	/**
	* Inserts _code into the map of justPressed buttons and removes it from the maps of justPressed and pressed buttons
	*
	* @param The button code
	*/
	void buttonUpListener(int _code);

	void buttonNullListener(int _code);

	/* _id is GLFW_JOYSTICK_1 through GLFW_JOYSTICK_LAST */
	Joystick(int _id, float _deadZone = 0.25f);
	~Joystick();
protected:

	/** Map of buttons which are currently pressed down */
	std::map<int, int> pressedButtons;
	/** Map of buttons which were pressed down since the joystick's last call to update */
	std::map<int, int> justPressedButtons;
	/** Map of buttons which were released since the joystick's last call to update */
	std::map<int, int> justReleasedButtons;
	
	/** Map of axes values since the joystick's last call to update */
	std::map<int, float> axesValues;
	/** Map of axes values previous to the joystick's last call to update */
	std::map<int, float> axesValuesPrev;


	// sets the button codes to correspond with xbox controller layout
	void initXbox();
	// sets the button codes to correspond with playstation controller layout
	void initPlaystation();
};