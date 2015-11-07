#pragma once

#include <node/NodeContent.h>
#include <scenario/Dialogue.h>
#include <scenario/Triggers.h>
#include <scenario/Option.h>

class Scenario;

class Conversation : public NodeContent{
private:
	// keeps track of the progress within this conversation
	unsigned long int currentDialogue;
	// the scenario this content belongs to
	Scenario * scenario;

public:
	std::string id;
	std::vector<Dialogue *> dialogueObjects;
	std::vector<NodeContent *> options;

	Conversation(Json::Value _json, Scenario * _scenario);
	~Conversation();

	// returns dialogueObjects.at(currentDialogue)
	Dialogue * getCurrentDialogue();
	// increments currentDialogue
	// returns true if there is a corresponding dialogue object
	// returns false if the end of the conversation has been reached
	bool sayNextDialogue();

	// sets the currentDialogue to the first valid dialogue object
	// also calls reset on all dialogue objects
	void reset();
};