#pragma once

#include "SayAction.h"
#include "GameJamCharacter.h"
#include "BitmapFont.h"
#include "Vox.h"

SayAction::SayAction(GameJamCharacter * _speaker, std::string _text, float _duration):
	DialogAction(_speaker),
	text(_text),
	duration(_duration),
	timePassed(0),
	setBlank(false),
	setText(false)
{
}

SayAction::~SayAction(){
}

DialogActionType SayAction::getActionType(){
	return SAY;
}

void SayAction::update(Step* _step){
	if(timePassed >= duration){
		complete = true;
		if(!setBlank){
			speaker->text->setText("");	
			setBlank = true;
		}
	}else{
		timePassed += vox::step.getDeltaTime();
		if(!setText){
			speaker->text->setText(text);
			setText = true;
		}
	}
}