#pragma once

#include <scenario/Conditions.h>
#include <scenario/Scenario.h>

#include <Log.h>

Condition * Condition::getCondition(Json::Value _json, Scenario * const _scenario){
	Condition * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "equality"){
		res = new ConditionEquality(_json);
	}else{
		throw "invalid trigger type";
	}
	res->scenario = _scenario;
	return res;
}

Condition::Condition(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString())
{
}

ConditionEquality::ConditionEquality(Json::Value _json) :
	Condition(_json),
	requiredValue(_json.get("value", "NO_REQUIRED_VALUE").asString())
{
}

bool ConditionEquality::evaluate(){
	// TODO: replace this with the new system
	/*bool res = scenario->characters.at(target)->variables.at(variable) == requiredValue;
	Log::info((res ? "Condition passed: " : "Condition failed: ") + target + "->" + variable + "("+scenario->characters.at(target)->variables.at(variable)+") == " + requiredValue);
	return res;*/
	return true;
}