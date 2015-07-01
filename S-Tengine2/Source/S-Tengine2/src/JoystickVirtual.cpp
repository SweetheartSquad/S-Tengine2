#pragma once

#include <JoystickVirtual.h>
#include <GLFW\glfw3.h>

JoystickVirtual::JoystickVirtual(int _id) :
	Joystick(_id),
	keyboard(&Keyboard::getInstance())
{
	name = "Virtual Joystick";
}

void JoystickVirtual::update(Step * _step){
	axesValues.clear();

	// left stick
	if(keyboard->keyJustDown(GLFW_KEY_W) && keyboard->keyJustDown(GLFW_KEY_S)){
		axesValues[xbox_axes::kLY] = 0.f;
	}else{
		if(keyboard->keyJustDown(GLFW_KEY_W)){
			axesValues[xbox_axes::kLY] = 1.f;
		}else if(keyboard->keyJustDown(GLFW_KEY_S)){
			axesValues[xbox_axes::kLY] = -1.f;
		}
	}

	if(keyboard->keyJustDown(GLFW_KEY_A) && keyboard->keyJustDown(GLFW_KEY_D)){
		axesValues[xbox_axes::kLX] = 0.f;
	}else{
		if(keyboard->keyJustDown(GLFW_KEY_A)){
			axesValues[xbox_axes::kLX] = -1.f;
		}else if(keyboard->keyJustDown(GLFW_KEY_D)){
			axesValues[xbox_axes::kLX] = 1.f;
		}
	}

	// right stick
	if(keyboard->keyJustDown(GLFW_KEY_I) && keyboard->keyJustDown(GLFW_KEY_K)){
		axesValues[xbox_axes::kRY] = 0.f;
	}else{
		if(keyboard->keyJustDown(GLFW_KEY_I)){
			axesValues[xbox_axes::kRY] = -1.f;
		}else if(keyboard->keyJustDown(GLFW_KEY_K)){
			axesValues[xbox_axes::kRY] = 1.f;
		}
	}

	if(keyboard->keyJustDown(GLFW_KEY_J) && keyboard->keyJustDown(GLFW_KEY_L)){
		axesValues[xbox_axes::kRX] = 0.f;
	}else{
		if(keyboard->keyJustDown(GLFW_KEY_J)){
			axesValues[xbox_axes::kRX] = -1.f;
		}else if(keyboard->keyJustDown(GLFW_KEY_L)){
			axesValues[xbox_axes::kRX] = 1.f;
		}
	}

	// bumpers
	if(keyboard->keyJustDown(GLFW_KEY_Q)){
		buttonDownListener(xbox_buttons::kL1);
	}else{
		buttonUpListener(xbox_buttons::kL1);
	}if(keyboard->keyJustDown(GLFW_KEY_E)){
		buttonDownListener(xbox_buttons::kR1);
	}else{
		buttonUpListener(xbox_buttons::kR1);
	}

	// face buttons
	if(keyboard->keyDown(GLFW_KEY_KP_8)){
		buttonDownListener(xbox_buttons::kY);
	}else{
		buttonUpListener(xbox_buttons::kY);
	}if(keyboard->keyDown(GLFW_KEY_KP_2)){
		buttonDownListener(xbox_buttons::kA);
	}else{
		buttonUpListener(xbox_buttons::kA);
	}if(keyboard->keyDown(GLFW_KEY_KP_4)){
		buttonDownListener(xbox_buttons::kX);
	}else{
		buttonUpListener(xbox_buttons::kX);
	}if(keyboard->keyDown(GLFW_KEY_KP_6)){
		buttonDownListener(xbox_buttons::kB);
	}else{
		buttonUpListener(xbox_buttons::kB);
	}
	/*if(keyboard->keyJustDown(GLFW_KEY_1)){
		buttonJustDown(xbox_buttons::kY);
	}if(keyboard->keyJustDown(GLFW_KEY_2)){
		buttonJustDown(xbox_buttons::kA);
	}if(keyboard->keyJustDown(GLFW_KEY_3)){
		buttonJustDown(xbox_buttons::kX);
	}if(keyboard->keyJustDown(GLFW_KEY_4)){
		buttonJustDown(xbox_buttons::kB);
	}*/
}