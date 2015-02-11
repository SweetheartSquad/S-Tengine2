#pragma once

#include "PuppetController.h"
#include "PuppetCharacter.h"
#include "Accelerometer.h"
#include "CharacterComponent.h"

PuppetController::PuppetController(Accelerometer* _accelerometer, PuppetCharacter* _puppetCharacter):
	NodeUpdatable(),
	accelerometer(_accelerometer),
	puppetCharacter(_puppetCharacter)
{
}

PuppetController::~PuppetController(){
}

void PuppetController::update(Step* _step){
	puppetCharacter->targetRoll = accelerometer->getRoll();
}