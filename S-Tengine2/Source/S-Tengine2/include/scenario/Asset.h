#pragma once

#include <node/NodeContent.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <Font.h>
#include <json\json.h>
#include <OpenALSound.h>

class Asset abstract : public virtual NodeContent, public virtual NodeLoadable{
public:
	std::string id;

	Asset(Json::Value _json);

	static Asset * getAsset(Json::Value _json);
};

class AssetTexture : public Asset{
public:
	Texture * texture;

	AssetTexture(Json::Value _json);
	~AssetTexture();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetTextureSampler : public Asset{
public:
	TextureSampler * textureSampler;

	AssetTextureSampler(Json::Value _json);
	~AssetTextureSampler();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetAudio : public Asset{
public:
	OpenAL_Sound * sound;

	AssetAudio(Json::Value _json);
	~AssetAudio();
	
	virtual void load() override;
	virtual void unload() override;
};

class AssetFont : public Asset{
public:
	Font * font;

	AssetFont(Json::Value _json);
	~AssetFont();
	
	virtual void load() override;
	virtual void unload() override;
};