#pragma once

#include <BulletController.h>

BulletController::BulletController() :
	disabledInputs(0),
	keyboard(Keyboard::getInstance()),
	mouse(Mouse::getInstance())
{
}

void BulletController::enable(){
	enabled = true;
}

void BulletController::disable(){
	enabled = false;
}

bool BulletController::isEnabled() const{
	return enabled;
}

void BulletController::update(Step * _step){
	handleInputs(_step, isEnabled() ? calculateInputs(_step) : disabledInputs);
}