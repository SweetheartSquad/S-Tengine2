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
	std::cout<<accelerometer->deltaX+accelerometer->deltaY<<"\n";
	if(accelerometer->deltaX + accelerometer->deltaY < -1 || accelerometer->deltaX + accelerometer->deltaY > 0
		&& !puppetCharacter->torso->movingVertically(1)) {
		if(puppetCharacter->torso->body->GetPosition().y < 8){
			float t = puppetCharacter->torso->body->GetAngle();
			puppetCharacter->torso->applyLinearImpulseUp(50*(1-sin(t)));
			if(puppetCharacter->torso->body->GetAngle() > 0){
				puppetCharacter->torso->applyLinearImpulseLeft(150*(1-cos(t)));
			}else{
				puppetCharacter->torso->applyLinearImpulseRight(150*(1-cos(t)));
			}
		}
	}else {
		puppetCharacter->targetRoll = accelerometer->getRoll();	
	}
}