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
	if(abs(accelerometer->x - accelerometer->lx) > 10.f 
		|| abs(accelerometer->y - accelerometer->ly) > 10.f 
		&& !puppetCharacter->torso->movingVertically(0.1f)) {

		//We can limit jumping based on y position but this won't work in levels such as repunzel
		if(puppetCharacter->canJump){
			puppetCharacter->canJump = false;
			float t = puppetCharacter->torso->body->GetAngle();
			puppetCharacter->torso->applyLinearImpulseUp(500*(1-sin(abs(t))));
			if(puppetCharacter->torso->body->GetAngle() > 0){
				puppetCharacter->torso->applyLinearImpulseLeft(350*(1-cos(t)));
			}else{
				puppetCharacter->torso->applyLinearImpulseRight(350*(1-cos(t)));
			}
		}
	}else{
		puppetCharacter->targetRoll = accelerometer->getRoll();	
	}
}