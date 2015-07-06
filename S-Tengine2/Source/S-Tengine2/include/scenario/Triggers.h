#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <functional>
#include <node/NodeContent.h>

class Scenario;

class Trigger abstract : public NodeContent{
public:
	Trigger(Scenario * _scenario);
	static Trigger * getTrigger(Json::Value _json, Scenario * _scenario);
	
	virtual void trigger() = 0;
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};

class TriggerSetVar : public Trigger{
public:
	std::string target;
	std::string variable;
	std::string newValue;

	virtual void trigger() override;
	TriggerSetVar(Json::Value _json, Scenario * _scenario);
};

class TriggerAddVar : public Trigger{
public:
	std::string target;
	std::string variable;
	std::string num;

	virtual void trigger() override;
	TriggerAddVar(Json::Value _json, Scenario * _scenario);
};

class TriggerSetConversation : public Trigger{
public:
	std::string newConversation;
	
	virtual void trigger() override;
	TriggerSetConversation(Json::Value _json, Scenario * _scenario);
};