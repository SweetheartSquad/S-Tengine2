#pragma once

#include <node/NodeUpdatable.h>

#include <map>
#include <string>
#include <queue>
#include <functional>

namespace sweet{

class Event : public Node{
private:
	std::map<std::string, std::string> dataString;
	std::map<std::string, float> dataFloat;
	std::map<std::string, int> dataInt;
public:
	std::string tag;
	
	int getIntData(std::string _key, int _default = 0) const;
	float getFloatData(std::string _key, float _default = 0) const;
	std::string getStringData(std::string _key, std::string _default = "") const;
	
	void setIntData(std::string _key, int _val);
	void setFloatData(std::string _key, float _val);
	void setStringData(std::string _key, std::string _val);

	Event(std::string _tag);
};

class EventManager : public NodeUpdatable{
private:
	// when an event is triggered, it will be placed into the event queue
	std::queue<Event *> events;
public:
	
	// when an event is consumed, it will access listeners[event->key]
	// and will call each function in the result, passing itself in as an argument
	// listeners are handled in order from first added to last added
	std::map<std::string, std::vector<std::function<void(Event *)>>> listeners;

	// triggers an event using the given object
	void triggerEvent(Event * _event);

	// triggers an empty event object with the provided tag
	void triggerEvent(std::string _tag);

	EventManager();
	~EventManager();

	virtual void update(Step * _step) override;
};

};