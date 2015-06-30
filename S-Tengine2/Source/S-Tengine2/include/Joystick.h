#pragma once

#include <node\NodeUpdatable.h>

#include <iostream>
#include <map>

/************************************************************
*
* encapsulating joystick events and properties
*
* TODO - Add modifier keys to the value section of the maps
*
*************************************************************/

class Joystick : public NodeUpdatable{
public:
	static enum xbox_buttons{
		kA,
		kB,
		kX,
		kY,
		kL1,
		kR1,
		kBACK,
		kSTART,
		kL3,
		kR3,
		kDPAD_UP,
		kDPAD_DOWN,
		kDPAD_LEFT,
		kDPAD_RIGHT
	};
	static enum xbox_axes{
		kLX,
		kLY,
		kBUMPERS,
		kRY,
		kRX
	};

	float deadZone;

	int id;
	std::string name;

	/**
	* Returns true if the given key is currently pressed down
	*
	* @param The button code - eg. button_mapping::xbox::kA
	* @return Whether the key is currently down or not
	*/
	bool buttonDown(int _code);

	/**
	* Returns true if the given key was released since the keyboard's last call to update
	*
	* @param The button code - eg. button_mapping::xbox::kA
	* @return Whether the key was released since the last keyboard update
	*/
	bool buttonJustUp(int _code);

	/**
	* Returns true if the given button was pressed down since the keyboard's last call to update
	*
	* @param The button code - eg. button_mapping::xbox::kA
	* @return Whether the key was pressed down since the last keyboard update
	*/
	bool buttonJustDown(int _code);

	// Returns the value of the given axis
	// If the absolute value is less than the deadZone, returns 0
	// If the axis doesn't exist, returns 0
	float getAxis(int _code);

	/**
	* Handles input from glfw
	*/
	virtual void update(Step * _step) override;

	/**
	* Inserts _code into the maps of justPressed and pressed buttons
	*
	* @param The button code - eg. button_mapping::xbox::kA
	*/
	void buttonDownListener(int _code);

	/**
	* Inserts _code into the map of justPressed buttons and removes it from the maps of justPressed and pressed buttons
	*
	* @param The button code - eg. button_mapping::xbox::kA
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
};