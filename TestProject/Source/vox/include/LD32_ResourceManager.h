#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>


class LD32_ResourceManager : public ResourceManager{
public:
	static SoundManager * music;

	static void init();
};