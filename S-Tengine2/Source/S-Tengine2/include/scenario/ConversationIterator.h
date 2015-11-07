#pragma once

#include <scenario/Conversation.h>

class Timeout;
class ConversationIterator : public Node{
public:
	// reference to the current conversation object (this will be nullptr when the iterator is not in use)
	Conversation * currentConversation;


	bool shouldSayNext;
	

	// whether the conversation is waiting on user input (i.e. the user needs to pick an option from an Ask object)
	bool waitingForInput;
	// if the current conversation is waiting on user input because of an Ask, this contains the options
	std::vector<void * > options;


	Timeout * autoProgressTimer;
	bool autoProgress;

	ConversationIterator();
	virtual ~ConversationIterator();

	// resets the current conversation, then sets it to nullptr
	void end();

	// moves the conversation forward by one step
	// if there is nothing left to say, returns false. Returns true otherwise
	virtual bool sayNext();
};