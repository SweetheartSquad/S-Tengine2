#pragma once

#include <scenario/Dialogue.h>
#include <scenario/Triggers.h>
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
		lines.push_back(textArray[i].asString());
	}
	const Json::Value triggersArray = _json["triggers"];
	for(int j = 0; j < triggersArray.size(); ++j){
		triggerCalls.push_back(Trigger::getTrigger(triggersArray[j], scenario));
	}
	const Json::Value conditionsArray = _json["conditions"];
	for(int i = 0; i < conditionsArray.size(); ++i){
		conditionChecks.push_back(Condition::getCondition(conditionsArray[i], scenario));
	}
}

Dialogue::~Dialogue(){
	while(triggerCalls.size() > 0){
		delete triggerCalls.back();
		triggerCalls.pop_back();
	}
	while(conditionChecks.size() > 0){
		delete conditionChecks.back();
		conditionChecks.pop_back();
	}
}

std::string Dialogue::getCurrentText(){
	return lines.at(currentText);
}

bool Dialogue::sayNextText(){
	++currentText;
	return currentText < lines.size();
}

bool Dialogue::evaluateConditions(){
	for(Condition * c : conditionChecks){
		if(!c->evaluate()){
			return false;
		}
	}
	return true;
}

void Dialogue::reset(){
	currentText = -1;
}

DialogueSay::DialogueSay(Json::Value _json, Scenario * _scenario) :
	Dialogue(_json, _scenario)
{
}


DialogueOption::DialogueOption(Json::Value _json, Scenario * _scenario) :
	text(_json["text"].asString())
{
	const Json::Value triggersArray = _json["triggers"];
	for(int i = 0; i < triggersArray.size(); ++i){
		triggers.push_back(Trigger::getTrigger(triggersArray[i], _scenario));
	}
}

DialogueOption::~DialogueOption(){
	while(triggers.size() > 0){
		delete triggers.back();
		triggers.pop_back();
	}
}

DialogueAsk::DialogueAsk(Json::Value _json, Scenario * _scenario) :
	Dialogue(_json, _scenario)
{
	const Json::Value optionsArray = _json["options"];
	for(int i = 0; i < optionsArray.size(); ++i){
		options.push_back(new DialogueOption(optionsArray[i], _scenario));
	}
}

DialogueAsk::~DialogueAsk(){
	while(options.size() > 0){
		delete options.back();
		options.pop_back();
	}
}


Dialogue * Dialogue::getDialogue(Json::Value _json, Scenario * _scenario){
	Dialogue * res = nullptr;
	
	if(_json.isMember("options")) {
		res = new DialogueAsk(_json, _scenario);
	}else {
		res = new DialogueSay(_json, _scenario);
	}

	return res;
}