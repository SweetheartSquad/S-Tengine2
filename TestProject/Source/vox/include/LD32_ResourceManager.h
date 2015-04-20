#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>

class LD32_ResourceManager : public ResourceManager{
public:
	static Texture * startSplash;
	static Texture * endSplash;
	static Texture * endSplashBad;
	static SoundManager * music;
	static SoundManager * sfx;
	static SoundManager * miscSounds;
	static SoundManager * bumperSfx;
	static SoundManager * enemySfx;
	static Texture * donutTop;
	static Texture * donutBot;

	static void init();
};