#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <node/NodeContent.h>
#include <EventManager.h>

class Scenario;
class Trigger;
class Condition;

class Dialogue : public NodeContent{
public:
	std::string speaker;
	std::vector<std::string> text;
	unsigned long int currentText;

	// events which will be triggered on this node's scenario when the dialogue is finished
	std::vector<sweet::Event *> triggers;
	std::vector<Condition *> conditions;
	
	virtual ~Dialogue();
	
	std::string getCurrentText();
	// increments currentText
	// returns true if there is a corresponding text to say
	// returns false if the end of the dialogue has been reached
	bool sayNextText();

	// loops through the dialogue's conditions
	// if any evaluate to false, returns false immediately
	// otherwise, returns true
	bool evaluateConditions();

	// loops through the dialogue's triggers
	// and forwards them to its scenario's EventManager
	void trigger();

	void reset();

	Dialogue(Json::Value _json, Scenario * _scenario);
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};