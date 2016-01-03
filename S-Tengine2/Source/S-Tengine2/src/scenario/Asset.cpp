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
	texture = new Texture(src, true, false, _json.get("generateMipmaps", true).asBool());
	//texture->load();
}
AssetTexture * AssetTexture::create(Json::Value _json){
	return new AssetTexture(_json);
}
AssetTexture::~AssetTexture(){
	delete texture;
}

void AssetTexture::load(){
	if(!loaded){
		texture->load();
	}
	Asset::load();
}

void AssetTexture::unload(){
	if(loaded){
		texture->unload();
	}
	Asset::unload();
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
AssetTextureSampler * AssetTextureSampler::create(Json::Value _json){
	return new AssetTextureSampler(_json);
}
AssetTextureSampler::~AssetTextureSampler(){
	delete textureSampler;
}

void AssetTextureSampler::load(){
	if(!loaded){
		textureSampler->load();
	}
	Asset::load();
}

void AssetTextureSampler::unload(){
	if(loaded){
		textureSampler->unload();
	}
	Asset::unload();
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
AssetAudio * AssetAudio::create(Json::Value _json){
	return new AssetAudio(_json);
}
AssetAudio::~AssetAudio(){
	delete sound;
}

void AssetAudio::load(){
	if(!loaded){
		sound->load();
	}
	Asset::load();
}

void AssetAudio::unload(){
	if(loaded){
		sound->unload();
	}
	Asset::unload();
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
AssetFont * AssetFont::create(Json::Value _json){
	return new AssetFont(_json);
}
AssetFont::~AssetFont(){
	delete font;
}

void AssetFont::load(){
	if(!loaded){
		font->load();
	}
	Asset::load();
}

void AssetFont::unload(){
	if(loaded){
		font->unload();
	}
	Asset::unload();
}

std::map<std::string, std::function<Asset * (Json::Value)>> Asset::creationRegistry;
bool Asset::registerType(std::string _typeName, std::function<Asset * (Json::Value)> _typeCreator){
	if(!creationRegistry.insert(std::make_pair(_typeName, _typeCreator)).second){
		Log::error("Asset type registration failed; type \"" + _typeName + "\" already registered.");
		return false;
	}
	return true;
}


Asset * Asset::getAsset(Json::Value _json){
	// retrieve the type string and use it to find the create function
	std::string type = _json.get("type", "NO_TYPE").asString();
	auto s = creationRegistry.find(type);
	
	// if the type is unregistered, log an error and return nullptr
	if(s == creationRegistry.end()){
		Log::error("Asset type \"" + type + "\" is unregistered; asset not allocated.");
		return nullptr;
	}

	// return the result of calling the create function with the provided json
	return s->second(_json);
}

// register asset types
bool Asset::registerTypes(){
	return 
		Asset::registerType("texture", &AssetTexture::create) &&
		Asset::registerType("textureSampler", &AssetTextureSampler::create) &&
		Asset::registerType("audio", &AssetAudio::create) &&
		Asset::registerType("font", &AssetFont::create);
}
const bool Asset::typesRegistered = Asset::registerTypes();