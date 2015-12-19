#pragma once

#include <node/NodeContent.h>
#include <node/NodeResource.h>

#include <scenario/Asset.h>
#include <scenario/Conditions.h>
#include <scenario/Conversation.h>
#include <scenario/Dialogue.h>

#include <vector>
#include <map>

#include <json/json.h>
#include <EventManager.h>

class Character : public NodeContent{
public:
	Character(Json::Value _json);
	std::string id;
	std::map<std::string, std::string> variables;
};

class Scenario : public virtual NodeContent, public virtual NodeResource{
private:
	static std::vector<Asset *> defaultAssets;
public:
	// events triggered by this scenario's dialogue will be handled by this manager
	sweet::EventManager eventManager;

	// the scenario filename
	std::string id;

	Json::Value root;
	std::map<std::string, Conversation *> conversations;
	std::map<std::string, Character *> characters;

	std::map<std::string, Asset *> assets;
	std::map<std::string, AssetTexture *> textures;
	std::map<std::string, AssetTextureSampler *> textureSamplers;
	std::map<std::string, AssetAudio *> audio;
	std::map<std::string, AssetFont *> fonts;

	static AssetTexture * defaultTexture;
	static AssetTextureSampler * defaultTextureSampler;
	static AssetAudio * defaultAudio;
	static AssetFont * defaultFont;
	// delete static variables used for default assets
	static void destruct();

	Conversation * currentConversation;
	
	Scenario(std::string _jsonSrc);
	~Scenario();
	
	AssetTexture * getTexture(std::string _id);
	AssetTextureSampler * getTextureSampler(std::string _id);
	AssetAudio * getAudio(std::string _id);
	AssetFont * getFont(std::string _id);

	virtual void load() override;
	virtual void unload() override;
};