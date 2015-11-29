#pragma once

#include <scenario/Dialogue.h>
#include <scenario/Conditions.h>
#include <scenario/Scenario.h>

#include <iostream>
#include <Log.h>

Dialogue::Dialogue(Json::Value _json, Scenario * _scenario) :
	currentText(-1),
	scenario(_scenario),
	speaker(_json.get("speaker", "NO_SPEAKER_DEFINED").asString())
{
	const Json::Value textArray = _json["text"];
	for(int i = 0; i < textArray.size(); ++i){
		text.push_back(textArray[i].asString());
	}
	const Json::Value triggersArray = _json["triggers"];
	for(int j = 0; j < triggersArray.size(); ++j){
		triggers.push_back(new sweet::Event(triggersArray[j]));
	}
	const Json::Value conditionsArray = _json["conditions"];
	for(int i = 0; i < conditionsArray.size(); ++i){
		conditions.push_back(Condition::getCondition(conditionsArray[i], scenario));
	}
}

Dialogue::~Dialogue(){
	while(triggers.size() > 0){
		delete triggers.back();
		triggers.pop_back();
	}
	while(conditions.size() > 0){
		delete conditions.back();
		conditions.pop_back();
	}
}

std::string Dialogue::getCurrentText(){
	return text.at(currentText);
}

bool Dialogue::sayNextText(){
	if(currentText == (unsigned long int)-1 || currentText < text.size() - 1){
		++currentText;
		return true;
	}else{
		return false;
	}
}

bool Dialogue::evaluateConditions(){
	for(Condition * c : conditions){
		if(!c->evaluate()){
			return false;
		}
	}
	return true;
}

void Dialogue::trigger(){
	for(sweet::Event * e : triggers){
		scenario->eventManager.triggerEvent(new sweet::Event(*e));
	}
}

void Dialogue::reset(){
	currentText = -1;
}