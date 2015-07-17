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

Scenario::Scenario(std::string _jsonSrc) :
	currentConversation(nullptr)
{
	

	Json::Reader reader;
	Json::Value defTexJson;
	bool parsingSuccessful;
	
	
	parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"texture\""
		"}", defTexJson);
	defaultTexture = new AssetTexture(defTexJson);

	parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"textureSampler\""
		"}", defTexJson);
	defaultTextureSampler = new AssetTextureSampler(defTexJson);
	
	parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"audio\""
		"}", defTexJson);
	defaultAudio = new AssetAudio(defTexJson);

	parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"font\""
		"}", defTexJson);
	defaultFont = new AssetFont(defTexJson);


	std::string jsonLoaded = FileUtils::voxReadFile(_jsonSrc);
	parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		
		 Json::Value charactersJson = root["characters"];
		 for(auto i = 0; i < charactersJson.size(); ++i) {
			Character * c = new Character(charactersJson[i]);
			characters[c->id] = c;
		 }

		 Json::Value conversationsJson = root["conversations"];
		 for(auto i = 0; i < conversationsJson.size(); ++i) {
			 Conversation * c = new Conversation(conversationsJson[i], this);
			 conversations[c->id] = c;
		 }

		 Json::Value texturesJson = root["assets"];
		 for(auto i = 0; i < texturesJson.size(); ++i) {
			 Asset * a = Asset::getAsset(texturesJson[i]);
			 AssetTexture * at = dynamic_cast<AssetTexture *>(a);
			 if(at != nullptr){
				textures[at->id] = at;
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

			 assets[a->id] = a;
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
	for(auto i : textures){
		delete i.second;
	}textures.clear();
	for(auto i : audio){
		delete i.second;
	}audio.clear();
	for(auto i : fonts){
		delete i.second;
	}fonts.clear();
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
	auto it = audio.find(_id);
	if(it != audio.end()){
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