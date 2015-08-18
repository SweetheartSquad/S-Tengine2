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
	std::vector<std::string> text;
	signed long int currentText;
	std::vector<Trigger *> triggers;
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

	void reset();

	static Dialogue * getDialogue(Json::Value _json, Scenario * _scenario);
protected:
	// the scenario this content belongs to
	Scenario * scenario;

	// this constructor is for internal use only; call Dialogue::getDialogue to create a new instance
	Dialogue(Json::Value _json, Scenario * _scenario);
};

class DialogueSay : public Dialogue{
public:
	DialogueSay(Json::Value _json, Scenario * _scenario);
};



class DialogueOption : public NodeContent{
public:
	// text to display to use for this option
	std::string text;
	std::vector<Trigger *> triggers;

	DialogueOption(Json::Value _json, Scenario * _scenario);
	~DialogueOption();
protected:
	// the scenario this content belongs to
	Scenario * scenario;
};
// a dialog object which prompts for user selection
class DialogueAsk : public Dialogue{
public:
	std::vector<DialogueOption *> options;
	
	DialogueAsk(Json::Value _json, Scenario * _scenario);
	~DialogueAsk();
};