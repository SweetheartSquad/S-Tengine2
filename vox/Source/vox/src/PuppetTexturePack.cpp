#pragma once

#include <PuppetTexturePack.h>

PuppetTexturePack::PuppetTexturePack(TextureSampler * _torso, TextureSampler * _armTex, TextureSampler * _headgearTex, TextureSampler * _headTex, TextureSampler * _handTex, TextureSampler * _faceTex) :
	headTex(_headTex),
	handTex(_handTex),
	faceTex(_faceTex),
	torsoTex(_torso),
	armTex(_armTex),
	headgearTex(_headgearTex)
	//NodeResource(true)
{
}