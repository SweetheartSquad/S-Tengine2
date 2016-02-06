#pragma once

#include <scenario/Conditions.h>
#include <scenario/Scenario.h>

#include <Log.h>

Condition * Condition::getCondition(Json::Value _json, Scenario * const _scenario){
	Condition * res = nullptr;
	res = new Condition(_json, _scenario);
	return res;
}

Condition::Condition(Json::Value _json, Scenario * _scenario) :
	event(new sweet::Event(_json)), 
	scenario(_scenario) {
}

bool Condition::evaluate() {
	if(scenario->conditionImplementations != nullptr &&
		scenario->conditionImplementations->find(event->tag) != scenario->conditionImplementations->end()) {
		return scenario->conditionImplementations->at(event->tag)(event);
	}
	ST_LOG_ERROR("No condition implementation found for condition " + event->tag);
	return false;
}
