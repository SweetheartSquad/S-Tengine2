#include "Texture.h"
#include <Resource.h>

Texture::Texture(const char* _src, int _width, int _height, bool _storeData):
	src(_src),
	width(_width),
	height(_height),
	data(nullptr),
	channels(nullptr)
{
	init();
}

Texture::~Texture(){
	delete channels;
}

void Texture::init(){
	if(channels == nullptr){	
		channels = new int(0);
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	unsigned char* tempData;

	if(data != nullptr){
		tempData = data;
	}else{
		tempData = Resource::loadImage(src, width, height, SOIL_LOAD_AUTO, channels);
		if(storeData){
			data = tempData;
		}
	}
	
	if(*channels == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempData);
	}else if(*channels == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
	}else{
		throw "dunno";
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	delete(channels);
}

void Texture::unload(){
	glDeleteTextures(1, &textureId);
	textureId = 0;
}