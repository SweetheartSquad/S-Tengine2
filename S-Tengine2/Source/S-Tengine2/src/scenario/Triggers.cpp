#pragma once

#include <scenario/Triggers.h>
#include <scenario/Scenario.h>

#include <Log.h>

Trigger * Trigger::getTrigger(Json::Value _json, Scenario * _scenario){
	Trigger * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "setVar"){
		res = new TriggerSetVar(_json, _scenario);
	}else if(type == "setConversation"){
		res = new TriggerSetConversation(_json, _scenario);
	}else if(type == "addVar"){
		res = new TriggerAddVar(_json, _scenario);
	}else{
		throw "invalid trigger type";
	}
	return res;
}

Trigger::Trigger(Scenario * _scenario) :
	scenario(_scenario)
{
}

TriggerSetVar::TriggerSetVar(Json::Value _json, Scenario * _scenario) :
	Trigger(_scenario),
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString()),
	newValue(_json.get("value", "NO_NEW_VALUE").asString())
{
}

void TriggerAddVar::trigger(){
	scenario->characters.at(target)->variables.at(variable) += num;
	Log::info("Add " + num + " to var: " + target + "->" + variable + " = " + scenario->characters.at(target)->variables.at(variable));
}

TriggerAddVar::TriggerAddVar(Json::Value _json, Scenario * _scenario) :
	Trigger(_scenario),
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString()),
	num(_json.get("num", "NO_NUM").asString())
{
}

void TriggerSetVar::trigger(){
	Log::info("Set Var: " + variable + " -> " + newValue);
	scenario->characters.at(target)->variables.at(variable) = newValue;
}

TriggerSetConversation::TriggerSetConversation(Json::Value _json, Scenario * _scenario) :
	Trigger(_scenario),
	newConversation(_json.get("conversation", "NO_NEW_CONVERSATION").asString())
{
}

void TriggerSetConversation::trigger(){
	Log::info("New Conversation: " + newConversation);
	scenario->currentConversation = scenario->conversations[newConversation];
	scenario->currentConversation->reset();
}