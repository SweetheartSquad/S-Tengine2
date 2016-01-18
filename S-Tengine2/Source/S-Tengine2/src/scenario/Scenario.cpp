#pragma once

#include <scenario/Scenario.h>
#include <scenario/Conversation.h>
#include <scenario/Dialogue.h>
#include <NineSlicing.h>

#include <Log.h>
#include <FileUtils.h>

Character::Character(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{
	const Json::Value vObj = _json["variables"];
	const Json::Value::Members vMembers = vObj.getMemberNames();
	for(auto v : vMembers){
		variables[v] = vObj[v].asString();
	}
}

AssetTexture * Scenario::defaultTexture = nullptr;
AssetTextureSampler * Scenario::defaultTextureSampler = nullptr;
AssetAudio * Scenario::defaultAudio = nullptr;
AssetFont * Scenario::defaultFont = nullptr;
AssetMesh * Scenario::defaultMesh = nullptr;

std::vector<Asset *> Scenario::defaultAssets;

void Scenario::destruct(){
	while(defaultAssets.size() > 0){
		delete defaultAssets.back();
		defaultAssets.pop_back();
	}
}

Scenario::Scenario(std::string _jsonSrc) :
	id(_jsonSrc),
	currentConversation(nullptr),
	NodeResource(false)
{
	Json::Reader reader;
	Json::Value defJson;
	bool parsingSuccessful;
	
	if(defaultTexture == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"texture\","
				"\"src\": \"../engine basics/img_cheryl.jpg\""
			"}", defJson);
		defaultTexture = AssetTexture::create(defJson, this);
		defaultAssets.push_back(defaultTexture);
	}
	
	if(defaultTextureSampler == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"textureSampler\","
				"\"src\": \"../engine basics/img_cheryl.jpg.def\""
			"}", defJson);
		defaultTextureSampler = AssetTextureSampler::create(defJson, this);
		defaultAssets.push_back(defaultTextureSampler);
	}
	
	if(defaultAudio == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"audio\","
				"\"src\": \"../engine basics/SCENE.ogg\""
			"}", defJson);
		defaultAudio = AssetAudio::create(defJson, this);
		defaultAssets.push_back(defaultAudio);
	}

	if(defaultFont == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"font\","
				"\"src\": \"../engine basics/OpenSans-Regular.ttf\""
			"}", defJson);
		defaultFont = AssetFont::create(defJson, this);
		defaultAssets.push_back(defaultFont);
	}

	if(defaultMesh == nullptr) {
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"mesh\","
				"\"src\": \"../engine basics/S-Tengine2_logo.obj\""
			"}", defJson);
		defaultMesh = AssetMesh::create(defJson, this);
		defaultAssets.push_back(defaultMesh);
	}

	std::string jsonLoaded = FileUtils::readFile(_jsonSrc);
	parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		
		 /*Json::Value charactersJson = root["characters"];
		 for(Json::Value::ArrayIndex i = 0; i < charactersJson.size(); ++i) {
			Character * c = new Character(charactersJson[i]);
			characters[c->id] = c;
		 }

		 Json::Value conversationsJson = root["conversations"];
		 for(Json::Value::ArrayIndex i = 0; i < conversationsJson.size(); ++i) {
			 Conversation * c = new Conversation(conversationsJson[i], this);
			 conversations[c->id] = c;
		 }*/

		 Json::Value texturesJson = root["assets"];
		 for(Json::Value::ArrayIndex i = 0; i < texturesJson.size(); ++i) {
			 Asset * a = Asset::getAsset(texturesJson[i], this);
			 assets[a->type][a->id] = a;
		 }
	}
}

Scenario::~Scenario(){
	/*for(auto i : conversations){
		delete i.second;
	}conversations.clear();
	for(auto i : characters){
		delete i.second;
	}characters.clear();*/
	for(auto a : assets){
		for(auto b : a.second){
			delete b.second;
		}
		a.second.clear();
	}assets.clear();
}

Asset * Scenario::getAsset(std::string _type, std::string _id){
	auto typeList = assets.find(_type);
	if(typeList == assets.end()){
		Log::warn("No asset of type \"" + _type + "\" found.");
		return nullptr;
	}
	auto a = typeList->second.find(_id);
	if(a == typeList->second.end()){
		Log::warn("No asset with the id \"" + _id + "\" found for type \"" + _type + "\".");
		return nullptr;
	}
	return a->second;
}

AssetTexture * Scenario::getTexture(std::string _id){
	AssetTexture * res = dynamic_cast<AssetTexture *>(getAsset("texture", _id));
	if(res == nullptr){
		Log::warn("Texture \"" + _id + "\" not found.");
		res = defaultTexture;
	}
	return res;
}

Texture_NineSliced * Scenario::getNineSlicedTexture(std::string _id){
	AssetTexture * res = dynamic_cast<AssetTexture *>(getAsset("texture", _id));
	if(res == nullptr){
		Log::warn("Texture \"" + _id + "\" not found.");
		res = defaultTexture;
	}
	return dynamic_cast<Texture_NineSliced *>(res->texture);
}

AssetTextureSampler * Scenario::getTextureSampler(std::string _id){
	AssetTextureSampler * res = dynamic_cast<AssetTextureSampler *>(getAsset("textureSampler", _id));
	if(res == nullptr){
		Log::warn("TextureSampler \"" + _id + "\" not found.");
		res = defaultTextureSampler;
	}
	return res;
}

AssetAudio * Scenario::getAudio(std::string _id){
	AssetAudio * res = dynamic_cast<AssetAudio *>(getAsset("audio", _id));
	if(res == nullptr){
		Log::warn("Audio \"" + _id + "\" not found.");
		res = defaultAudio;
	}
	return res;
}

AssetFont * Scenario::getFont(std::string _id){
	AssetFont * res = dynamic_cast<AssetFont *>(getAsset("font", _id));
	if(res == nullptr){
		Log::warn("Font \"" + _id + "\" not found.");
		res = defaultFont;
	}
	return res;
}

AssetConversation * Scenario::getConversation(std::string _id){
	AssetConversation * res = dynamic_cast<AssetConversation *>(getAsset("conversation", _id));
	if(res == nullptr){
		Log::error("Conversation \"" + _id + "\" not found.");
	}
	return res;
}

AssetMesh* Scenario::getMesh(std::string _id) {
	AssetMesh * res = dynamic_cast<AssetMesh *>(getAsset("mesh", _id));
	if(res == nullptr){
		Log::error("Mesh \"" + _id + "\" not found.");
		res = defaultMesh;
	}
	return res;
}

void Scenario::load(){
	if(!loaded){
		for(auto a : assets){
			for(auto b : a.second){
				b.second->load();
			}
		}
	}
	NodeResource::load();
}

void Scenario::unload(){
	if(loaded){
		for(auto a : assets){
			for(auto b : a.second){
				b.second->unload();
			}
		}
	}
	NodeResource::unload();
}