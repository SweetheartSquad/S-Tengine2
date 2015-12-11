#pragma once

#include <scenario/Asset.h>
#include <FileUtils.h>
#include <json/json.h>

Asset::Asset(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{

}

AssetTexture::AssetTexture(Json::Value _json) :
	Asset(_json),
	texture(nullptr)
{
	std::string src = _json.get("src", "NO_TEXTURE").asString();
	if(src == "NO_TEXTURE"){
		Log::warn("Loaded texture without a src: Cheryl substitution in effect.");
		src = "assets/engine basics/img_cheryl.jpg";
	}else{
		src = "assets/textures/" + src;
	}
	texture = new Texture(src, true, false);
	texture->load();
}
AssetTexture::~AssetTexture(){
	delete texture;
}

AssetTextureSampler::AssetTextureSampler(Json::Value _json) :
	Asset(_json),
	textureSampler(nullptr)
{
	std::string defTex = "assets/engine basics/img_cheryl.jpg";
	std::string src =  _json.get("src", "NO_TEXTURE").asString();
	if(src == "NO_TEXTURE"){
		Log::warn("Could not parse texture sampler definition, using cheryl default");	
		src = "assets/engine basics/img_cheryl.jpg.def";
	}else{
		src = "assets/textures/" + src;
	}

	const std::string defJsonRaw = FileUtils::readFile(src);

	Json::Reader reader;
	Json::Value defJson;
	bool parsingSuccessful;
	
	parsingSuccessful = reader.parse(defJsonRaw, defJson);

	if(parsingSuccessful){
		float u		  = defJson.get("u", 0).asFloat();
		float v		  = defJson.get("v", 0).asFloat();
		float w		  = defJson.get("w", 256).asFloat();
		float h		  = defJson.get("h", 256).asFloat();
		std::string t = defJson.get("t", defTex).asString();
		std::string defPath = src.substr(0, src.find_last_of("\\/"));
		Texture * texture = new Texture(defPath + "/" + t, true, false);
		textureSampler = new TextureSampler(texture, w, h, u, v);
		textureSampler->load();
	}
}
AssetTextureSampler::~AssetTextureSampler(){
	delete textureSampler;
}

AssetAudio::AssetAudio(Json::Value _json) :
	Asset(_json)
{
	std::string src = _json.get("src", "NO_AUDIO").asString();
	if(src == "NO_AUDIO"){
		Log::warn("Loaded audio without a src: SCENE substitution in effect.");
		src = "assets/engine basics/SCENE.ogg";
	}else{
		src = "assets/audio/" + src;
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

AssetFont::AssetFont(Json::Value _json) :
	Asset(_json)
{
	std::string src = _json.get("src", "NO_FONT").asString();
	if(src == "NO_FONT"){
		Log::warn("Loaded font without a src: Open Sans substitution in effect.");
		src = "assets/engine basics/OpenSans-Regular.ttf";
	}else{
		src = "assets/fonts/" + src;
	}
	int size = _json.get("size", 24).asInt();
	font = new Font(src, size, false);
}
AssetFont::~AssetFont(){
	delete font;
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
	}else if(type == "font"){
		res = new AssetFont(_json);
	}else{
		throw "invalid asset type";
	}
	return res;
}