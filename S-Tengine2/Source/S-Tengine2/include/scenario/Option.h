#pragma once

#include <node/NodeContent.h>
#include <scenario/Conversation.h>
#include <scenario/Triggers.h>

class Option : public NodeContent{
public:
	std::string text;
	std::string link;

	Option(Json::Value _json, Scenario * _scenario);
	~Option();
};