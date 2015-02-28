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

	if(accelerometer->pitch > 1.5f && accelerometer->z > 550.0f){
		puppetCharacter->action();
	}
	if(abs(accelerometer->x - accelerometer->lx) > 10.f 
		|| abs(accelerometer->y - accelerometer->ly) > 10.f) {
		if(puppetCharacter->canJump){
			float t = puppetCharacter->torso->body->GetAngle();
			puppetCharacter->torso->applyLinearImpulseUp(200);
			if(puppetCharacter->torso->body->GetAngle() > 0){
				puppetCharacter->torso->applyLinearImpulseLeft(100*(1-cos(t)));
			}else{
				puppetCharacter->torso->applyLinearImpulseRight(100*(1-cos(t)));
			}
		}
	}
	puppetCharacter->targetRoll = accelerometer->getRoll();	
}