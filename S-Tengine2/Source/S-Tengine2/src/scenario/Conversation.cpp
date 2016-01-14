#pragma once

#include <scenario/Scenario.h>
#include <scenario/Conversation.h>
#include <Log.h>
#include <assert.h>




Option::Option(Json::Value _json, Scenario * const _scenario) :
	text(_json.get("text", "").asString()),
	link(_json.get("link", "END").asString())
{
}

Option::~Option(){

} 



Conversation::Conversation(Json::Value _json, Scenario * const _scenario) :
	id(_json.get("id", "NO_ID").asString()),
	currentDialogue(-1),
	scenario(_scenario)
{
	Json::Value convoDialogueJson = _json["dialogue"];

	for(Json::Value::ArrayIndex j = 0; j < convoDialogueJson.size(); ++j){
		dialogueObjects.push_back(new Dialogue(convoDialogueJson[j], scenario));
	}

	Json::Value convoOptionsJson = _json["options"];

	for(Json::Value::ArrayIndex j = 0; j < convoOptionsJson.size(); ++j){
		options.push_back(new Option(convoOptionsJson[j], scenario));
	}
}

Conversation::~Conversation(){
	while(dialogueObjects.size() > 0){
		delete dialogueObjects.back();
		dialogueObjects.pop_back();
	}
}

Dialogue * Conversation::getCurrentDialogue(){
	return dialogueObjects.at(currentDialogue);
}

bool Conversation::sayNextDialogue(){

	// if we're at the start, move to the first valid dialogue
	if(currentDialogue == (unsigned long int) -1){
		return advance();
	}

	// if there's still text in the current dialogue object,
	// we don't increment and instead return true to indicate
	// that the conversation is still going
	if(dialogueObjects.at(currentDialogue)->sayNextText()){
		return true;
	}
	
	// trigger anything left on the current dialogue object before moving on
	// (note that if there are multiple lines of text for the object, the triggers will be called multiple times)
	if(getCurrentDialogue()->currentText+1 == getCurrentDialogue()->text.size()){
		getCurrentDialogue()->trigger();
	}

	// if there aren't any dialogue objects left, the conversation is over so return false
	if(currentDialogue+1 >= dialogueObjects.size()){
		return false;
	}
	
	// go to next dialogue
	return advance();
}


bool Conversation::advance(){
	// check that we still have something to say
	// check if we are allowed to say the current selection
	bool valid = false;
	do{
		++currentDialogue;
		// if any conditions are untrue for a given dialogue object, skip over it
		valid = dialogueObjects.at(currentDialogue)->evaluateConditions();
	}while(currentDialogue+2 < dialogueObjects.size() && !valid);
	
	if(valid){
		// advance to the first text in the new dialogue object
		return dialogueObjects.at(currentDialogue)->sayNextText();
	}else{
		// no valid dialogue objects were found past this point
		return false;
	}
}


void Conversation::reset(){
	Log::info("Reset conversation: " + id);
	for(Dialogue * d : dialogueObjects){
		d->reset();
	}

	currentDialogue = -1;
}



#include <Timeout.h>
ConversationIterator::ConversationIterator() :
	currentConversation(nullptr),
	autoProgress(false),
	autoProgressTimer(new Timeout(2.f, [this](sweet::Event * _event){
		/*if(dialogue->ticking){
			dialogue->finishTicking();
			autoProgressTimer->restart();
		}else{*/
			this->shouldSayNext = true;
		//}
	})),
	waitingForInput(false),
	shouldSayNext(false)
{;
}

ConversationIterator::~ConversationIterator(){
	delete autoProgressTimer;
}

void ConversationIterator::select(unsigned long int _option){
	if(!waitingForInput){
		throw "you can't do that";
	}
	currentConversation = currentConversation->scenario->getConversation(currentConversation->options.at(_option)->link)->conversation;
	currentConversation->reset();
	waitingForInput = false;
	sayNext();
}

bool ConversationIterator::sayNext(){

	
	if(waitingForInput){
		throw "you can't do that";
	}
	if(currentConversation == nullptr){
		return false;
	}


	
	// move conversation forward
	bool stuffLeftToSay = currentConversation->sayNextDialogue();
	// allow any events triggered during conversation to be handled
	currentConversation->scenario->eventManager.update(nullptr);
	
	if(!stuffLeftToSay){
		// if there's nothing left to say, check for options
		if(currentConversation->options.size() > 1){
			// multiple options means wait for user input
			waitingForInput = true;
		}else if(currentConversation->options.size() == 1){
			// single option means go to the link
			currentConversation = currentConversation->scenario->getConversation(currentConversation->options.front()->link)->conversation;
			currentConversation->reset();
			sayNext();
		}else{
			// no options means conversation over, return false
			end();
			return false;
		}
	}


	// if auto-progression is enabled, restart the timer (should also set the duration here based on length of content)
	if(!waitingForInput){
 		autoProgressTimer->restart();
	}

	//std::cout << currentConversation->getCurrentDialogue()->getCurrentText() << std::endl;

	return true;
}

void ConversationIterator::end(){
	currentConversation = nullptr;
}