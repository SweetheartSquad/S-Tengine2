#pragma once

#include <scenario/Dialogue.h>
#include <scenario/Conditions.h>
#include <scenario/Scenario.h>

#include <iostream>
#include <Log.h>

Dialogue::Dialogue(Json::Value _json, Scenario * const _scenario) :
	currentText(-1),
	scenario(_scenario),
	speaker(_json.get("speaker", "NO_SPEAKER_DEFINED").asString())
{
	const Json::Value textArray = _json["text"];
	for(Json::Value i : textArray){
		text.push_back(i.asString());
	}
	const Json::Value triggersArray = _json["triggers"];
	for(Json::Value i : triggersArray){
		triggers.push_back(new sweet::Event(i));
	}
	const Json::Value conditionsArray = _json["conditions"];
	for(Json::Value i : conditionsArray){
		conditions.push_back(Condition::getCondition(i, scenario));
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
	if(currentText == (unsigned long int)-1 || currentText+1 < text.size()){
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