#pragma once

#include <scenario/Asset.h>

Asset::Asset(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{

}

AssetTexture::AssetTexture(Json::Value _json) :
	Asset(_json),
	width(_json.get("width", 256).asInt()),
	height(_json.get("height", 256).asInt()),
	texture(nullptr)
{
	std::string src = _json.get("src", "NO_TEXTURE").asString();
	if(src == "NO_TEXTURE"){
		src = "../assets/engine basics/img_cheryl.jpg";
		Log::warn("Loaded texture without a src: Cheryl substitution in effect.");
	}else{
		src = "../assets/textures/" + src;
	}
	texture = new Texture(src, width, height, true, false);
	texture->load();
}
AssetTexture::~AssetTexture(){
	delete texture;
}

AssetTextureSampler::AssetTextureSampler(Json::Value _json) :
	Asset(_json),
	textureSampler(nullptr)
{
	std::string src = _json.get("src", "NO_TEXTURE").asString();
	if(src == "NO_TEXTURE"){
		src = "../assets/engine basics/img_cheryl.jpg";
		Log::warn("Loaded textureSampler without a src: Cheryl substitution in effect.");
	}else{
		src = "../assets/textures/" + src;
	}
	int width = _json.get("width", 256).asInt();
	int height = _json.get("height", 256).asInt();
	float u = _json.get("u", 0).asFloat();
	float v = _json.get("v", 0).asFloat();
	float w = _json.get("w", 256).asFloat();
	float h = _json.get("h", 256).asFloat();
	Texture * texture = new Texture(src, width, height, true, false);
	textureSampler = new TextureSampler(texture, w, h, u, v);
	textureSampler->load();
}
AssetTextureSampler::~AssetTextureSampler(){
	delete textureSampler;
}

AssetAudio::AssetAudio(Json::Value _json) :
	Asset(_json)
{
	std::string src = _json.get("src", "NO_AUDIO").asString();
	if(src == "NO_AUDIO"){
		src = "../assets/engine basics/SCENE.ogg";
		Log::warn("Loaded audio without a src: SCENE substitution in effect.");
	}else{
		src = "../assets/audio/" + src;
	}
	bool stream = _json.get("stream", false).asBool();
	if(stream){
		sound = new OpenAL_SoundStream(src.c_str(), false, false);
	}else{
		sound = new OpenAL_SoundSimple(src.c_str(), false, false);
	}
}
AssetAudio::~AssetAudio(){
	delete sound;
}

Asset * Asset::getAsset(Json::Value _json){
	Asset * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "texture"){
		res = new AssetTexture(_json);
	}else if(type == "textureSampler"){
		res = new AssetTextureSampler(_json);
	}else if(type == "audio"){
		res = new AssetAudio(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}