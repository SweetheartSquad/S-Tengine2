#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <node/NodeContent.h>
#include <EventManager.h>

class Scenario;

class Condition: public NodeContent{
public:

	static Condition * getCondition(Json::Value _json, Scenario * const _scenario);
	
	sweet::Event * event; 

	explicit Condition(Json::Value _json, Scenario * _scenario);
	~Condition();

	// returns true if the condition's criteria are met
	virtual bool evaluate();
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};
