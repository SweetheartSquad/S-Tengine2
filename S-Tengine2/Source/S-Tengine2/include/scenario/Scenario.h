#pragma once

#include <node/NodeContent.h>
#include <node/NodeResource.h>

#include <scenario/Asset.h>
#include <scenario/Conditions.h>
#include <scenario/Conversation.h>
#include <scenario/Dialogue.h>
#include <scenario/Triggers.h>

#include <vector>
#include <map>

#include <json/json.h>

class Character : public NodeContent{
public:
	Character(Json::Value _json);
	std::string id;
	std::map<std::string, std::string> variables;
};

class Scenario : public NodeContent{
public:	

	Json::Value root;
	std::map<std::string, Conversation *> conversations;
	std::map<std::string, Character *> characters;
	std::map<std::string, AssetTexture *> textures;
	std::map<std::string, AssetAudio *> audio;

	Conversation * currentConversation;
	
	Scenario(std::string _jsonSrc);
	~Scenario();
	
	AssetTexture * getTexture(std::string _id);
	AssetAudio * getAudio(std::string _id);
};