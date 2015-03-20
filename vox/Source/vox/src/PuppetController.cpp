#pragma once

#include "PuppetController.h"
#include "PuppetCharacter.h"
#include "Accelerometer.h"
#include "CharacterComponent.h"

std::vector<bool> PuppetController::idsAvailable = std::vector<bool>(4, true);

void PuppetController::setPuppetCharacter(PuppetCharacter * _puppetCharacter){
	puppetCharacter = _puppetCharacter;
	for (unsigned long int i = 0; i < idsAvailable.size(); ++i){
		if (idsAvailable.at(i)){
			puppetCharacter->id = i;
			idsAvailable.at(i) = false;
			break;
		}
	}
}

void PuppetController::unassign(){
	if (puppetCharacter != nullptr){
		idsAvailable.at(puppetCharacter->id) = true;
		puppetCharacter->id = -1;
		puppetCharacter = nullptr;
	}
}

PuppetController::PuppetController(Accelerometer* _accelerometer):
	NodeUpdatable(),
	accelerometer(_accelerometer),
	puppetCharacter(nullptr)
{
}

PuppetController::~PuppetController(){
}

void PuppetController::update(Step* _step){
	if(puppetCharacter != nullptr){
		if(!puppetCharacter->ai){
			if(accelerometer->pitch > 1.5f && accelerometer->z > 550.0f){
				puppetCharacter->action();
			}
			if(abs(accelerometer->x - accelerometer->lx) > 15.f 
				|| abs(accelerometer->y - accelerometer->ly) > 15.f) {
				puppetCharacter->jump();
			}
			puppetCharacter->targetRoll = accelerometer->getRoll();
		}
	}
}