#pragma once

#include <scenario/Scenario.h>
#include <scenario/Conversation.h>
#include <scenario/Dialogue.h>

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
	Json::Value defTexJson;
	bool parsingSuccessful;
	
	if(defaultTexture == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"texture\","
				"\"src\": \"../engine basics/img_cheryl.jpg\""
			"}", defTexJson);
		defaultTexture = new AssetTexture(defTexJson);
		defaultAssets.push_back(defaultTexture);
	}
	
	if(defaultTextureSampler == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"textureSampler\","
				"\"src\": \"../engine basics/img_cheryl.jpg.def\""
			"}", defTexJson);
		defaultTextureSampler = new AssetTextureSampler(defTexJson);
		defaultAssets.push_back(defaultTextureSampler);
	}
	
	if(defaultAudio == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"audio\","
				"\"src\": \"../engine basics/SCENE.ogg\""
			"}", defTexJson);
		defaultAudio = new AssetAudio(defTexJson);
		defaultAssets.push_back(defaultAudio);
	}

	if(defaultFont == nullptr){
		parsingSuccessful = reader.parse(
			"{"
				"\"id\":\"DEFAULT\","
				"\"type\": \"font\","
				"\"src\": \"../engine basics/OpenSans-Regular.ttf\""
			"}", defTexJson);
		defaultFont = new AssetFont(defTexJson);
		defaultAssets.push_back(defaultFont);
	}

	std::string jsonLoaded = FileUtils::readFile(_jsonSrc);
	parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		
		 Json::Value charactersJson = root["characters"];
		 for(Json::Value::ArrayIndex i = 0; i < charactersJson.size(); ++i) {
			Character * c = new Character(charactersJson[i]);
			characters[c->id] = c;
		 }

		 Json::Value conversationsJson = root["conversations"];
		 for(Json::Value::ArrayIndex i = 0; i < conversationsJson.size(); ++i) {
			 Conversation * c = new Conversation(conversationsJson[i], this);
			 conversations[c->id] = c;
		 }

		 Json::Value texturesJson = root["assets"];
		 for(Json::Value::ArrayIndex i = 0; i < texturesJson.size(); ++i) {
			 Asset * a = Asset::getAsset(texturesJson[i]);
			 assets[a->id] = a;

			 AssetTexture * at = dynamic_cast<AssetTexture *>(a);
			 if(at != nullptr){
				textures[at->id] = at;
				continue;
			 }
			 AssetTextureSampler * ats = dynamic_cast<AssetTextureSampler *>(a);
			 if(ats != nullptr){
				textureSamplers[ats->id] = ats;
				continue;
			 }
			 AssetAudio * aa = dynamic_cast<AssetAudio *>(a);
			 if(aa != nullptr){
				 audio[aa->id] = aa;
				 continue;
			 }
			 AssetFont * af = dynamic_cast<AssetFont *>(a);
			 if(af != nullptr){
				 fonts[af->id] = af;
				 continue;
			 }
		 }
	}
}

Scenario::~Scenario(){
	for(auto i : conversations){
		delete i.second;
	}conversations.clear();
	for(auto i : characters){
		delete i.second;
	}characters.clear();
	for(auto i : assets){
		delete i.second;
	}assets.clear();
}

AssetTexture * Scenario::getTexture(std::string _id){
	AssetTexture * res = nullptr;
	auto it = textures.find(_id);
	if(it != textures.end()){
		res = it->second;
	}else{
		res = defaultTexture;
	}
	return res;
}

AssetTextureSampler * Scenario::getTextureSampler(std::string _id){
	AssetTextureSampler * res = nullptr;
	auto it = textureSamplers.find(_id);
	if(it != textureSamplers.end()){
		res = dynamic_cast<AssetTextureSampler *>(it->second);
	}else{
		res = defaultTextureSampler;
	}
	return res;
}

AssetAudio * Scenario::getAudio(std::string _id){
	AssetAudio * res = nullptr;
	auto it = audio.find(_id);
	if(it != audio.end()){
		res = it->second;
	}else{
		res = defaultAudio;
	}
	return res;
}

AssetFont * Scenario::getFont(std::string _id){
	AssetFont * res = nullptr;
	auto it = fonts.find(_id);
	if(it != fonts.end()){
		res = it->second;
	}else{
		res = defaultFont;
	}
	return res;
}