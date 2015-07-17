#pragma once

#include <TextureSampler.h>
#include <Texture.h>
#include <FileUtils.h>
#include <Log.h>

#include <json\json.h>

TextureSampler::TextureSampler(Texture * _texture, float _width, float _height, float _u, float _v) :
	NodeResource(true),
	texture(_texture),
	width(_width),
	height(_height),
	u(_u),
	v(_v),
	releaseTexture(_texture->autoRelease)
{
	++texture->referenceCount;
}
TextureSampler::TextureSampler(std::string _definitionDir, std::string _definitionName, bool _releaseTexture) :
	NodeResource(true),
	texture(nullptr),
	width(0),
	height(0),
	u(0),
	v(0),
	releaseTexture(_releaseTexture)
{
	if(!_definitionName.empty()){
		std::string jsonString = FileUtils::voxReadFile(_definitionDir + _definitionName);
		Json::Value root;
		Json::Reader reader;
		bool parsedSuccess = reader.parse(jsonString, root);
		if(!parsedSuccess){
			Log::error("Unable to parse TextureSampler definition: " + _definitionDir + _definitionName);	
		}
		
		u = root.get("u", 0).asFloat();
		v = root.get("v", 0).asFloat();
		width = root.get("w", 0).asFloat();
		height = root.get("h", 0).asFloat();
		float s = root.get("s", 0).asFloat();
		std::string tex = _definitionDir;
		tex.append(root.get("t", "NO_TEXTURE").asString());
		texture = new Texture(tex, s, s, true, _releaseTexture);
		++texture->referenceCount;
	}
}

TextureSampler::~TextureSampler(){
	texture->decrementAndDelete();
}

void TextureSampler::load(){
	texture->load();
	NodeResource::load();
}

void TextureSampler::unload(){
	texture->unload();
	NodeResource::unload();
}