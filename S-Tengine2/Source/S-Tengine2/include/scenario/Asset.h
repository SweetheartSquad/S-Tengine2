#pragma once

#include <node/NodeContent.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <json\json.h>
#include <OpenALSound.h>

class Asset abstract : public NodeContent{
public:
	std::string id;

	Asset(Json::Value _json);

	static Asset * getAsset(Json::Value _json);
};

class AssetTexture : public Asset{
public:
	float width;
	float height;
	Texture * texture;

	AssetTexture(Json::Value _json);
	~AssetTexture();
};

class AssetTextureSampler : public Asset{
public:
	TextureSampler * textureSampler;

	AssetTextureSampler(Json::Value _json);
	~AssetTextureSampler();
};

class AssetAudio : public Asset{
public:
	OpenAL_Sound * sound;

	AssetAudio(Json::Value _json);
	~AssetAudio();
};