#pragma once

#include <EventManager.h>

sweet::Event::Event(std::string _tag) :
	tag(_tag)
{
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
		delete events.back();
		events.pop();
	}
}

void sweet::EventManager::triggerEvent(std::string _tag){
	events.push(new Event(_tag));
}

void sweet::EventManager::triggerEvent(Event * _event){
	events.push(_event);
}

void sweet::EventManager::update(Step * _step){
	while(events.size() > 0){
		sweet::Event * e = events.back();
		for(std::function<void(sweet::Event *)> f : listeners[e->tag]){
			f(e);
		}
		delete e;
		events.pop();
	}
}