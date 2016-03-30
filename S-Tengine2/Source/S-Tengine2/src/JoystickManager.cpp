#pragma once

#include <JoystickManager.h>
#include <JoystickVirtual.h>

JoystickManager::JoystickManager(float _deadZone) :
	deadZone(_deadZone)
{
	for(unsigned long int i = 0; i < GLFW_JOYSTICK_LAST; ++i){
		joysticks[i] = nullptr;
	}
	update(nullptr);
}

JoystickManager::~JoystickManager(){
	for(unsigned long int i = 0; i < GLFW_JOYSTICK_LAST; ++i){
		delete joysticks[i];
		joysticks[i] = nullptr;
	}
}

void JoystickManager::update(Step * _step){
	for(unsigned long int i = 0; i < GLFW_JOYSTICK_LAST; ++i){
		// update controllers
		if(joysticks[i] != nullptr){
			joysticks[i]->update(_step);
		}
	}
}

void JoystickManager::reconnect() {
	for(unsigned long int i = 0; i < GLFW_JOYSTICK_LAST; ++i){
		// handle connections
		if(glfwJoystickPresent(i)){
			if(joysticks[i] == nullptr){
				joysticks[i] = new Joystick(i, deadZone);
			}
		}else{
			if(joysticks[i] != nullptr){
				JoystickVirtual * j = dynamic_cast<JoystickVirtual *>(joysticks[i]);
				if(j == nullptr){
					delete joysticks[i];
					joysticks[i] = nullptr;
				}
			}
		}
	}
}