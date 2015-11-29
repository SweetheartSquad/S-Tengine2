#pragma once

#include <node/NodeContent.h>
#include <scenario/Dialogue.h>

class Option : public NodeContent{
public:
	std::string text;
	std::string link;

	Option(Json::Value _json, Scenario * _scenario);
	~Option();
};




class Scenario;

class Conversation : public NodeContent{
private:
	// keeps track of the progress within this conversation
	unsigned long int currentDialogue;

	bool advance();
public:
	// the scenario this content belongs to
	Scenario * scenario;

	std::vector<Option *> options;
	std::string id;
	std::vector<Dialogue *> dialogueObjects;
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

class Timeout;
class ConversationIterator : public Node{
public:
	// reference to the current conversation object (this will be nullptr when the iterator is not in use)
	Conversation * currentConversation;


	bool shouldSayNext;
	
	// whether the conversation is waiting on user input (i.e. the user needs to pick an option from an Ask object)
	bool waitingForInput;

	Timeout * autoProgressTimer;
	bool autoProgress;

	ConversationIterator();
	virtual ~ConversationIterator();

	// sets the current conversation to nullptr
	void end();

	// moves the conversation forward by one step
	// if there is nothing left to say, returns false. Returns true otherwise
	virtual bool sayNext();

	// selects _option from the currentConversation's list of options
	// throws an exception if not waiting for input
	void select(unsigned long int _option);
};