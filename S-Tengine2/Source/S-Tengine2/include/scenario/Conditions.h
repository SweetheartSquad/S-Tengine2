#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <node/NodeContent.h>

class Scenario;

class Condition abstract: public NodeContent{
public:
	static Condition * getCondition(Json::Value _json, Scenario * const _scenario);
	
	std::string target;
	std::string variable;

	Condition(Json::Value _json);

	// returns true if the condition's criteria are met
	virtual bool evaluate() = 0;
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};

class ConditionEquality : public Condition{
public:
	std::string requiredValue;

	ConditionEquality(Json::Value _json);
	virtual bool evaluate() override;
};
