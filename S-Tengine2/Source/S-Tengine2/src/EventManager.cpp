#pragma once

#include <EventManager.h>

sweet::Event::Event(const char * _tag) :
	tag(std::string(_tag))
{
}
sweet::Event::Event(std::string _tag) :
	tag(_tag)
{
}
sweet::Event::Event(Json::Value _json) :
	tag(_json["type"].asString())
{
	Json::Value argsJson = _json["args"];
	Json::Value::Members argsJsonMembers = argsJson.getMemberNames();

	// TODO: figure out if there's a way to distinguish between int and double in the json

	for(auto j : argsJsonMembers){
		Json::Value v = argsJson[j]["value"];
		if(v.isDouble()){
			setFloatData(j, v.asDouble());
		}else if(v.isInt()){
			setIntData(j, v.asInt());
		}else if(v.isString()){
			setStringData(j, v.asString());
		}
	}
}

int sweet::Event::getIntData(std::string _key, int _default) const{
	std::map<std::string, int>::const_iterator res = dataInt.find(_key);
	return (res == dataInt.end()) ? _default : res->second;
}

float sweet::Event::getFloatData(std::string _key, float _default) const{
	std::map<std::string, float>::const_iterator res = dataFloat.find(_key);
	return (res == dataFloat.end()) ? _default : res->second;
}

std::string sweet::Event::getStringData(std::string _key, std::string _default) const{
	std::map<std::string, std::string>::const_iterator res = dataString.find(_key);
	return (res == dataString.end()) ? _default : res->second;
}

void sweet::Event::setIntData(std::string _key, int _val){
	dataInt[_key] = _val;
}
void sweet::Event::setFloatData(std::string _key, float _val){
	dataFloat[_key] = _val;
}
void sweet::Event::setStringData(std::string _key, std::string _val){
	dataString[_key] = _val;
}

sweet::EventManager::EventManager(){
}

sweet::EventManager::~EventManager(){
	while(events.size() > 0){
		delete events.front();
		events.pop();
	}

	while(childManagers.size() > 0){
		removeChildManager(childManagers.back());
	}

	while(parentManagers.size() > 0){
		removeParentManager(parentManagers.back());
	}
}

void sweet::EventManager::triggerEvent(std::string _tag){
	events.push(new sweet::Event(_tag));
}

void sweet::EventManager::triggerEvent(sweet::Event * _event){
	events.push(_event);
}

void sweet::EventManager::addEventListener(std::string _tag, std::function<void (Event *)> _listener ){
	listeners[_tag].push_back(_listener);
}

void sweet::EventManager::handle(sweet::Event * _event){
	std::vector<std::function<void(sweet::Event *)>> l = listeners[_event->tag];
	for(std::function<void(sweet::Event *)> f : l){
		f(_event);
	}
	for(sweet::EventManager * m : parentManagers){
		m->handle(_event);
	}
}

void sweet::EventManager::update(Step * _step){
	while(events.size() > 0){
		sweet::Event * e = events.front();
		handle(e);

		delete e;
		events.pop();
	}
}



void sweet::EventManager::addChildManager(sweet::EventManager * _childManager){
	// Add the child to the list of children and set it's parent to this
	childManagers.push_back(_childManager);
	_childManager->addParentManager(this);
}

signed long int sweet::EventManager::removeChildManager(sweet::EventManager * _childManager){
	for(signed long int i = childManagers.size()-1; i >= 0; --i){
		if(_childManager == childManagers.at(i)){
			childManagers.erase(childManagers.begin() + i);
			_childManager->removeParentManager(this);
			return i;
		}
	}
	return -1;
}

void sweet::EventManager::removeParentManager(sweet::EventManager * _parentManager){
	for(signed long int i = parentManagers.size()-1; i >= 0; --i){
		if(parentManagers.at(i) == _parentManager){
			parentManagers.erase(parentManagers.begin() + i);
			return;
		}
	}
}

void sweet::EventManager::addParentManager(sweet::EventManager * _parentManager){
	parentManagers.push_back(_parentManager);
}