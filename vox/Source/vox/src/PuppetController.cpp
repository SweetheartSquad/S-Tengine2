#pragma once

#include "PuppetController.h"

PuppetController::PuppetController(Accelerometer* _accelerometer, PuppetCharacter* _puppetCharacter):
	NodeUpdatable(),
	accelerometer(_accelerometer),
	puppetCharacter(_puppetCharacter)
{
}

PuppetController::~PuppetController(){
}

void PuppetController::update(Step* _step){
}