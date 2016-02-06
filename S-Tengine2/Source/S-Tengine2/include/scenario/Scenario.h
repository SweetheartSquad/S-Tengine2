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
#include <Functional>

class Character : public NodeContent{
public:
	Character(Json::Value _json);
	std::string id;
	std::map<std::string, std::string> variables;
};

class Texture_NineSliced;

typedef std::map<std::string, std::function<bool(sweet::Event *)>> ConditionImplementations;

class Scenario : public virtual NodeContent, public virtual NodeResource{
private:
public:

	ConditionImplementations * conditionImplementations;

	static std::vector<Asset *> defaultAssets;
	// events triggered by this scenario's dialogue will be handled by this manager
	sweet::EventManager eventManager;

	// the scenario filename
	std::string id;

	Json::Value root;
	
	// accessed using "assets[type][id]"
	std::map<std::string, std::map<std::string, Asset *>> assets;

	static AssetTexture * defaultTexture;
	static AssetTextureSampler * defaultTextureSampler;
	static AssetAudio * defaultAudio;
	static AssetFont * defaultFont;
	static AssetMesh * defaultMesh;
	// delete static variables used for default assets
	static void destruct();

	Conversation * currentConversation;
	
	Scenario(std::string _jsonSrc);
	~Scenario();
	
	// returns the asset of the specified type with the specified id
	// returns nullptr if not found (note that no default substitution is used when calling this function directly)
	Asset * getAsset(std::string _type, std::string _id);
	
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetTexture * getTexture(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	Texture_NineSliced * getNineSlicedTexture(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetTextureSampler * getTextureSampler(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetAudio * getAudio(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetFont * getFont(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetConversation * getConversation(std::string _id);
	// convenience function: just calls getAsset with a specific type and casts the result; if not found uses the default asset for that type
	AssetMesh * getMesh(std::string _id);

	virtual void load() override;
	virtual void unload() override;
};