#pragma once

#include <scenario/Conversation.h>
#include <Log.h>
#include <assert.h>

Conversation::Conversation(Json::Value _json, Scenario * _scenario) :
	id(_json.get("id", "NO_ID").asString()),
	currentDialogue(0),
	scenario(_scenario)
{
	Json::Value convoDialogueJson = _json["dialogue"];

	for(auto j = 0; j < convoDialogueJson.size(); ++j){
		dialogueObjects.push_back(Dialogue::getDialogue(convoDialogueJson[j], scenario));
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
	// if there aren't any dialogue objects left, the conversation is over so return false
	if(currentDialogue >= dialogueObjects.size()){
		return false;
	}

	// if there's still text in the current dialogue object,
	// we don't increment and instead return true to indicate
	// that the conversation is still going
	if(dialogueObjects.at(currentDialogue)->sayNextText()){
		return true;
	}


	// go to next dialogue
	// check that we still have something to say
	// check if we are allowed to say the current selection
	bool valid = false;
	while(currentDialogue < dialogueObjects.size()-1 && !valid){
		// if any conditions are untrue for a given dialogue object, skip over it
		valid = dialogueObjects.at(++currentDialogue)->evaluateConditions();
	}
	if(valid){
		// get the first text in the new dialogue object
		return dialogueObjects.at(currentDialogue)->sayNextText();
	}else{
		// no valid dialogue objects were found past this one
		return false;
	}
}

void Conversation::reset(){
	Log::info("Reset conversation: " + id);
	for(Dialogue * d : dialogueObjects){
		d->reset();
	}

	currentDialogue = 0;
	while(!dialogueObjects.at(currentDialogue)->evaluateConditions()){
		++currentDialogue;
		assert(currentDialogue < dialogueObjects.size());
	}
}



#include <Timeout.h>
ConversationIterator::ConversationIterator() :
	currentConversation(nullptr),
	autoProgress(false),
	autoProgressTimer(new Timeout(2.f)),
	waitingForInput(false),
	shouldSayNext(false)
{
	autoProgressTimer->onCompleteFunction = [this](Timeout * _this){
		/*if(dialogue->ticking){
			dialogue->finishTicking();
			autoProgressTimer->restart();
		}else{*/
			this->shouldSayNext = true;
		//}
	};
}

ConversationIterator::~ConversationIterator(){
	delete autoProgressTimer;
}

bool ConversationIterator::sayNext(){
	if(currentConversation == nullptr){
		return false;
	}
	// trigger anything left on the current dialogue object
	// (note that if there are multiple lines of text for the object, the triggers will be called multiple times)
	if(currentConversation->getCurrentDialogue()->currentText == currentConversation->getCurrentDialogue()->text.size()-1){
		for(Trigger * t : currentConversation->getCurrentDialogue()->triggers){
			t->trigger();
		}
	}

	// move conversation forward
	if(!currentConversation->sayNextDialogue()){
		// if there's nothing left to say, return false
		return false;
	}

	// set the speaker
	//std::string sp = currentConversation->getCurrentDialogue()->speaker;
	//speaker->setText(std::wstring(sp.begin(), sp.end()));
	//WAG_ResourceManager::speaker = sp;


	// set the images
	//loadFrame(currentConversation->getCurrentDialogue()->portrait);
	//loadPortrait(currentConversation->getCurrentDialogue()->speaker);
	
	// set the text
	//std::string thingToSay = currentConversation->getCurrentDialogue()->getCurrentText();
	//dialogue->setText(std::wstring(thingToSay.begin(), thingToSay.end()));
	//dialogue->tickerIn(0.05f);

	// check for dialogue options
	DialogueAsk * ask = dynamic_cast<DialogueAsk *>(currentConversation->getCurrentDialogue());
	if(ask != nullptr){
		if(ask->currentText == ask->text.size()-1){
			waitingForInput = true;
			//removeChild(progressButton);
			for(unsigned long int i = 0; i < ask->options.size(); ++i){
				//dialogue->appendText(std::wstring(s.begin(), s.end()));
				//TextArea * o = new TextArea(world, scene, font, textShader, 0.3f);
				//if(i != 0){
				//	o->setMarginLeft(10);
				//}
				//std::wstringstream ss;
				//ss << (i+1) << L". " << std::wstring(ask->options.at(i).begin(), ask->options.at(i).end());
				//o->setText(ss.str());
				//options.push_back(o);
				//optionslayout->addChild(o);
				//o->parents.at(0)->scale(50,50,1);
				
				/*std::vector<Trigger *> optionResult = ask->optionsResults.at(i);
				o->onClickFunction = [this, optionResult](NodeUI * _this) {
					// remove the function pointers on all the other buttons to avoid multiple buttons getting triggered at the same time
					for(auto o : options){
						if(o != _this){
							o->onClickFunction = nullptr;
						}
					}

					for(auto t : optionResult){
						t->trigger();
					}
					this->waitingForInput = false;
					this->shouldSayNext = true;
					this->addChild(progressButton);
				};*/
			}
		}
	}

	// if auto-progression is enabled, restart the timer (should also set the duration here based on length of content)
	if(!waitingForInput){
 		autoProgressTimer->restart();
	}

	std::cout << currentConversation->getCurrentDialogue()->getCurrentText() << std::endl;

	return true;
}