#pragma once

#include <node/NodeContent.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <scenario/Conversation.h>
#include <Font.h>
#include <json\json.h>
#include <OpenALSound.h>

class Asset abstract : public virtual NodeContent, public virtual NodeLoadable{
private:
	// a map which associates the functions for creating different asset types with their type name
	// used in order to dynamically instantiate different types of assets based on a string specifying the type
	static std::map<std::string, std::function<Asset * (Json::Value, Scenario * const)>> creationRegistry;
protected:
	Scenario * scenario;
public:
	std::string id;
	std::string type;

	Asset(Json::Value _json, Scenario * const _scenario);

	// checks _json for a type member
	// type is used to query the creationRegistry in order to dynamically instantiate an asset
	// if the type is unregistered, returns nullptr
	static Asset * getAsset(Json::Value _json, Scenario * const _scenario);

	// registers a function for creating a specific asset type under the specified _typeName
	// if the type has already been registered, logs an error and returns false
	// returns true otherwise
	static bool registerType(std::string _typeName, std::function<Asset * (Json::Value, Scenario * const)> _typeCreator);
};




/*
// Template for new asset type:
class AssetTYPE : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetTYPE(Json::Value _json, Scenario * const _scenario);
public:

	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetTYPE * create(Json::Value _json, Scenario * const _scenario);
	~AssetTYPE();
	
	virtual void load() override;
	virtual void unload() override;
};
*/


class AssetTexture : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetTexture(Json::Value _json, Scenario * const _scenario);
public:
	Texture * texture;

	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetTexture * create(Json::Value _json, Scenario * const _scenario);
	~AssetTexture();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetTextureSampler : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetTextureSampler(Json::Value _json, Scenario * const _scenario);
public:
	TextureSampler * textureSampler;
	
	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetTextureSampler * create(Json::Value _json, Scenario * const _scenario);
	~AssetTextureSampler();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetAudio : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetAudio(Json::Value _json, Scenario * const _scenario);
public:
	OpenAL_Sound * sound;

	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetAudio * create(Json::Value _json, Scenario * const _scenario);
	~AssetAudio();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetFont : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetFont(Json::Value _json, Scenario * const _scenario);
public:
	Font * font;

	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetFont * create(Json::Value _json, Scenario * const _scenario);
	~AssetFont();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetConversation : public Asset{
private:
	// constructor is private; use create instead if you need to instantiate directly
	AssetConversation(Json::Value _json, Scenario * const _scenario);
public:
	Conversation * conversation;

	// substitute for public constructor (we can't take the address of the constructor,
	// so we have a static function which simply returns a new instance of the class instead)
	static AssetConversation * create(Json::Value _json, Scenario * const _scenario);
	~AssetConversation();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetMesh : public Asset {
private:
	AssetMesh(Json::Value _json, Scenario * const _scenario);
public:
	std::vector<MeshInterface *> * meshes;
	
	static AssetMesh * create(Json::Value _json, Scenario * const _scenario);
	~AssetMesh();

	virtual void load() override;
	virtual void unload() override;
};
