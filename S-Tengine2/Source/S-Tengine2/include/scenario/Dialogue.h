#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <node/NodeContent.h>

class Scenario;
class Trigger;
class Condition;

class Dialogue abstract : public NodeContent{
public:
	std::string speaker;
	std::string portrait;
	std::vector<std::string> text;
	signed long int currentText;
	std::vector<Trigger *> triggers;
	std::vector<Condition *> conditions;
	
	Dialogue(Json::Value _json, Scenario * _scenario);
	~Dialogue();
	
	std::string getCurrentText();
	// increments currentText
	// returns true if there is a corresponding text to say
	// returns false if the end of the dialogue has been reached
	bool sayNextText();

	// loops through the dialogue's conditions
	// if any evaluate to false, returns false immediately
	// otherwise, returns true
	bool evaluateConditions();

	void reset();

	static Dialogue * getDialogue(Json::Value _json, Scenario * _scenario);
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};

class DialogueSay : public Dialogue{
public:
	DialogueSay(Json::Value _json, Scenario * _scenario);
};


// a dialog object which prompts for user selection
class DialogueAsk : public Dialogue{
public:
	std::vector<std::string> options;
	std::vector<std::vector<Trigger *>> optionsResults;

	DialogueAsk(Json::Value _json, Scenario * _scenario);
	~DialogueAsk();
};