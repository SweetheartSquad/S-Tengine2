#pragma once

#include <scenario/Asset.h>
#include <FileUtils.h>
#include <json/json.h>
#include <Resource.h>
#include <MeshFactory.h>

Asset::Asset(Json::Value _json, Scenario * const _scenario) :
	id(_json.get("id", "NO_ID").asString()),
	type(_json.get("type", "NO_TYPE").asString()),
	scenario(_scenario)
{

}

AssetTexture::AssetTexture(Json::Value _json, Scenario * const _scenario) :
	Asset(_json, _scenario),
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
AssetTexture * AssetTexture::create(Json::Value _json, Scenario * const _scenario){
	return new AssetTexture(_json, _scenario);
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

AssetTextureSampler::AssetTextureSampler(Json::Value _json, Scenario * const _scenario) :
	Asset(_json, _scenario),
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
AssetTextureSampler * AssetTextureSampler::create(Json::Value _json, Scenario * const _scenario){
	return new AssetTextureSampler(_json, _scenario);
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

AssetAudio::AssetAudio(Json::Value _json, Scenario * const _scenario) :
	Asset(_json, _scenario)
{
	std::string src = _json.get("src", "NO_AUDIO").asString();
	if(src == "NO_AUDIO"){
		Log::warn("Loaded audio without a src: SCENE substitution in effect.");
		src = "assets/engine basics/SCENE.ogg";
	}else{
		src = "assets/audio/" + src;
	}
	bool stream = _json.get("stream", false).asBool();
	std::string category = _json.get("category", "other").asString();
	if(stream){
		sound = new OpenAL_SoundStream(src.c_str(), false, false, category);
	}else{
		sound = new OpenAL_SoundSimple(src.c_str(), false, false, category);
	}
}
AssetAudio * AssetAudio::create(Json::Value _json, Scenario * const _scenario){
	return new AssetAudio(_json, _scenario);
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

AssetFont::AssetFont(Json::Value _json, Scenario * const _scenario) :
	Asset(_json, _scenario)
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
AssetFont * AssetFont::create(Json::Value _json, Scenario * const _scenario){
	return new AssetFont(_json, _scenario);
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




AssetConversation::AssetConversation(Json::Value _json, Scenario * const _scenario) :
	Asset(_json, _scenario),
	conversation(new Conversation(_json, _scenario))
{
}
AssetConversation * AssetConversation::create(Json::Value _json, Scenario * const _scenario){
	return new AssetConversation(_json, _scenario);
}
AssetConversation::~AssetConversation(){
	delete conversation;
}

void AssetConversation::load(){
	if(!loaded){
	}
	Asset::load();
}

AssetMesh::AssetMesh(Json::Value _json, Scenario* const _scenario) :
	Asset(_json, _scenario)
{
	meshes = new std::vector<MeshInterface *>();
	std::string src = _json.get("src", "NO_MESH").asString();
	if(src == "NO_MESH"){
		Log::warn("Loaded mesh without a src: Cube substitution in effect.");
		meshes->push_back(MeshFactory::getCubeMesh());
	}else{
		src = "assets/meshes/" + src;
		auto loadedMeshes = Resource::loadMeshFromObj(src);
		meshes->insert(meshes->end(), loadedMeshes.begin(), loadedMeshes.end());
	}
}

AssetMesh* AssetMesh::create(Json::Value _json, Scenario* const _scenario) {
	return new AssetMesh(_json, _scenario);
}

AssetMesh::~AssetMesh() {
	for(auto mesh : *meshes) {
		delete mesh;
	}
	delete meshes;
}

void AssetMesh::load() {
	if(!loaded) {
		for(auto mesh : *meshes) {
			mesh->load();
		}
	}
	Asset::load();
}

void AssetMesh::unload() {
	if(loaded) {
		for(auto mesh : *meshes) {
			mesh->unload();
		}
	}
	Asset::unload();
}

void AssetConversation::unload(){
	if(loaded){
	}
	Asset::unload();
}




std::map<std::string, std::function<Asset * (Json::Value, Scenario * const)>> Asset::creationRegistry;
bool Asset::registerType(std::string _typeName, std::function<Asset * (Json::Value, Scenario * const _scenario)> _typeCreator){
	if(!creationRegistry.insert(std::make_pair(_typeName, _typeCreator)).second){
		Log::error("Asset type registration failed; type \"" + _typeName + "\" already registered.");
		return false;
	}
	return true;
}


Asset * Asset::getAsset(Json::Value _json, Scenario * const _scenario){
	// retrieve the type string and use it to find the create function
	std::string type = _json.get("type", "NO_TYPE").asString();
	auto s = creationRegistry.find(type);
	
	// if the type is unregistered, log an error and return nullptr
	if(s == creationRegistry.end()){
		Log::error("Asset type \"" + type + "\" is unregistered; asset not allocated.");
		return nullptr;
	}

	// return the result of calling the create function with the provided json
	return s->second(_json, _scenario);
}

// register asset types during static initialization
static bool registerTypes(){
	return 
		Asset::registerType("texture", &AssetTexture::create) &&
		Asset::registerType("textureSampler", &AssetTextureSampler::create) &&
		Asset::registerType("audio", &AssetAudio::create) &&
		Asset::registerType("font", &AssetFont::create) &&
		Asset::registerType("conversation", &AssetConversation::create) &&
		Asset::registerType("mesh", &AssetMesh::create);
}
static bool typesRegistered = registerTypes();