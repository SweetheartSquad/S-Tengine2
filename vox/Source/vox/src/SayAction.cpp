#pragma once

#include "SayAction.h"
#include "Character.h"
#include "BitmapFont.h"
#include "Vox.h"

SayAction::SayAction(Character * _speaker, std::string _text, float _duration):
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
			spreaker->text->setText("");	
			setBlank = true;
		}
	}else{
		timePassed += vox::step.getDeltaTime();
		if(!setText){
			spreaker->text->setText(text);
			setText = true;
		}
	}
}