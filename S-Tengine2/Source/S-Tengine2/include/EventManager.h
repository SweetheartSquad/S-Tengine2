#pragma once

#include <node/NodeUpdatable.h>

#include <map>
#include <string>
#include <queue>
#include <functional>
#include <json/json.h>

// 

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
	
	// empty event with the given tag
	explicit Event(const char * _tag);
	// empty event with the given tag
	explicit Event(std::string _tag);
	// event filled from a json value
	// the tag is the "type" attribute
	// and the data is filled from the "args" attribute
	explicit Event(Json::Value _json);
};

class EventManager : public NodeUpdatable{
private:
	// when an event is triggered, it will be placed into the event queue
	std::queue<sweet::Event *> events;

	// when an event is handled on child managers, the event will be handled by this manager afterwards
	std::vector<sweet::EventManager *> childManagers;
	// when an event is handled by this manager, the event will be handled by parent managers afterwards
	std::vector<sweet::EventManager *> parentManagers;
	
	void removeParentManager(sweet::EventManager * _parentManager);
	void addParentManager(sweet::EventManager * _parentManager);

	// calls the listeners for _event->tag and passes in _event as an argument
	// then calls handle on the parent managers
	void handle(sweet::Event * _event);
public:
	// function pointer which takes a pointer to an event as an argument and returns void
	typedef std::function<void (sweet::Event *)> Listener;
	
	// when an event is consumed, it will access listeners[event->key]
	// and will call each function in the result, passing itself in as an argument
	// listeners are handled in order from first added to last added
	std::map<std::string, std::vector<Listener>> listeners;

	// triggers an event using the given object
	void triggerEvent(sweet::Event * _event);

	// triggers an empty event object with the provided tag
	void triggerEvent(std::string _tag);

	// listeners are usually in the format [captures](sweet::Event * _event){ code; }
	void addEventListener(std::string _tag, Listener _listener );

	EventManager();
	~EventManager();

	virtual void update(Step * _step) override;


	// adds a child manager to this manager
	// after child managers handle events, they handled by their parents
	void addChildManager(EventManager * _childManager);
	// removes a child manager from this manager
	// returns the index of the child in the children list, or -1 if it wasn't a child
	signed long int removeChildManager(EventManager * _childManager);
};

}; // end namespace sweet