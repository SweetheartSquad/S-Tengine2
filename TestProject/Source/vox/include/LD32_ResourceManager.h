#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>

class LD32_ResourceManager : public ResourceManager{
public:
	static SoundManager * music;
	static SoundManager * sfx;
	static Texture * donutTop;
	static Texture * donutBot;

	static void init();
};