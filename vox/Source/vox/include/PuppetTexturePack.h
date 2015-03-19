#pragma once

//#include <node\NodeResource.h>
#include <PuppetResourceManager.h>

class TextureSampler;

class PuppetTexturePack/* : public NodeResource*/{
public:
	PuppetTexturePack(TextureSampler * _torso = nullptr, TextureSampler * _armTex = nullptr, TextureSampler * _headgearTex = nullptr, TextureSampler * _headTex = PuppetResourceManager::head1, TextureSampler * _handTex = PuppetResourceManager::hand1, TextureSampler * _faceTex = PuppetResourceManager::face1, float _scale = 1.0f);

	TextureSampler * headTex;
	TextureSampler * handTex;
	TextureSampler * faceTex;
	TextureSampler * torsoTex;
	TextureSampler * armTex;
	TextureSampler * headgearTex;
	float scale;
	/*virtual void load() override;
	virtual void unload() override;*/
};