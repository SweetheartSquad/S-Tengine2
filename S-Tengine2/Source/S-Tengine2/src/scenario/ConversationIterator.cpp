#pragma once

#include <scenario/ConversationIterator.h>
#include <Timeout.h>
#include <iostream>

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
	if(currentConversation->getCurrentDialogue()->currentText == currentConversation->getCurrentDialogue()->lines.size()-1){
		for(Trigger * t : currentConversation->getCurrentDialogue()->triggerCalls){
			t->trigger();
		}
	}

	// move conversation forward
	if(!currentConversation->sayNextDialogue()){
		// if there's nothing left to say check for options
		if(options.size() > 0){
			waitingForInput = true;
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
		}else{
			// no more dialogue and no options, so return false
			return false;
		}
	}

	// if auto-progression is enabled, restart the timer (should also set the duration here based on length of content)
	if(!waitingForInput){
 		autoProgressTimer->restart();
	}

	std::cout << currentConversation->getCurrentDialogue()->getCurrentText() << std::endl;

	return true;
}