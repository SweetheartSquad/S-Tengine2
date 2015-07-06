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
	bool parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"texture\","
			"\"width\": 256,"
			"\"height\": 256"
		"}", defTexJson);
	textures["DEFAULT"] = new AssetTexture(defTexJson);
	parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"audio\","
			"\"stream\": false"
		"}", defTexJson);
	audio["DEFAULT"] = new AssetAudio(defTexJson);


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
}

AssetTexture * Scenario::getTexture(std::string _id){
	AssetTexture * res = nullptr;
	auto it = textures.find(_id);
	if(it != textures.end()){
		res = it->second;
	}else{
		res = textures["DEFAULT"];
	}
	return res;
}

AssetAudio * Scenario::getAudio(std::string _id){
	AssetAudio * res = nullptr;
	auto it = audio.find(_id);
	if(it != audio.end()){
		res = it->second;
	}else{
		res = audio["DEFAULT"];
	}
	return res;
}