#pragma once

#include <Keyboard.h>

Keyboard::Keyboard() :
	shift(false),
	control(false),
	alt(false),
	super(false)
{
}

Keyboard::~Keyboard(){
}

bool Keyboard::keyJustUp(int _glfwKeyCode) const{
	return justReleasedKeys.find(_glfwKeyCode) != justReleasedKeys.end();
}

bool Keyboard::keyJustDown(int _glfwKeyCode) const{
	return justPressedKeys.find(_glfwKeyCode) != justPressedKeys.end();
}

bool Keyboard::keyDown(int _glfwKeyCode) const{
	return justPressedKeys.find(_glfwKeyCode) != justPressedKeys.end() || pressedKeys.find(_glfwKeyCode) != pressedKeys.end();
}

void Keyboard::keyDownListener(int _glfwKeyCode){
	justPressedKeys.insert(std::make_pair(_glfwKeyCode, _glfwKeyCode));
	pressedKeys.insert(std::make_pair(_glfwKeyCode, _glfwKeyCode));
}

void Keyboard::keyUpListener(int _glfwKeyCode){
	justReleasedKeys.insert(std::make_pair(_glfwKeyCode, _glfwKeyCode));

	if(pressedKeys.find(_glfwKeyCode) != pressedKeys.end()){
		pressedKeys.erase(_glfwKeyCode);
	}

	if(justPressedKeys.find(_glfwKeyCode) != justPressedKeys.end()){
		justPressedKeys.erase(_glfwKeyCode);
	}
}

void Keyboard::update(){
	justPressedKeys.clear();
	justReleasedKeys.clear();
}

Keyboard & Keyboard::getInstance(){
	static Keyboard *keyboard;
	if(keyboard == 0){
		keyboard= new Keyboard();
	}
	return *keyboard;
}