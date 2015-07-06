#pragma once

#include <scenario/Dialogue.h>
#include <scenario/Triggers.h>
#include <scenario/Conditions.h>
#include <scenario/Scenario.h>

#include <iostream>
#include <Log.h>

Dialogue::Dialogue(Json::Value _json, Scenario * _scenario) :
	currentText(-1),
	scenario(_scenario)
{
	speaker = _json.get("speaker", "NO_SPEAKER_DEFINED").asString();
	portrait = _json.get("portrait", "NO_PORTRAIT_DEFINED").asString();
	const Json::Value textArray = _json["text"];
	for(int i = 0; i < textArray.size(); ++i){
		text.push_back(textArray[i].asString());
	}
	const Json::Value triggersArray = _json["triggers"];
	for(int j = 0; j < triggersArray.size(); ++j){
		triggers.push_back(Trigger::getTrigger(triggersArray[j], scenario));
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
	++currentText;
	return currentText < text.size();
}

bool Dialogue::evaluateConditions(){
	for(Condition * c : conditions){
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

DialogueAsk::DialogueAsk(Json::Value _json, Scenario * _scenario) :
	Dialogue(_json, _scenario)
{
	const Json::Value optionsArray = _json["options"];
	for(int i = 0; i < optionsArray.size(); ++i){
		options.push_back(optionsArray[i]["text"].asString());
		optionsResults.push_back(std::vector<Trigger *>());

		const Json::Value triggersArray = optionsArray[i]["triggers"];
		for(int j = 0; j < triggersArray.size(); ++j){
			optionsResults.back().push_back(Trigger::getTrigger(triggersArray[j], _scenario));
		}
	}
}

DialogueAsk::~DialogueAsk(){
	while(optionsResults.size() > 0){
		while(optionsResults.back().size() > 0){
			delete optionsResults.back().back();
			optionsResults.back().pop_back();
		}
		optionsResults.pop_back();
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