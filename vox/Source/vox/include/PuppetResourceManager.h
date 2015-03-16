#pragma once

#include <ResourceManager.h>

class Texture;
class TextureSampler;

class PuppetResourceManager abstract : public ResourceManager{
public:	
	static Texture * blank;
	static Texture * stageFloor;
	static Texture * sky;
	static Texture * ground1;
	static Texture * tree1; 
	static Texture * tree2;	
	static Texture * bush1;
	static Texture * bush2;

	static TextureSampler * head1;
	static TextureSampler * face1;
	static TextureSampler * hand1;
	
	static TextureSampler * countDown0;
	static TextureSampler * countDown1;
	static TextureSampler * countDown2;
	static TextureSampler * countDown3;
	static TextureSampler * countDown4;
	static TextureSampler * countDown5;

	
	static void init();
};