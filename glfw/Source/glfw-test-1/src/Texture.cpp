#pragma once

#include "Texture.h"
#include <Resource.h>
#include <GLUtils.h>
#include <SOIL.h>

Texture::Texture(const char* _src, int _width, int _height, bool _storeData, bool _autoRelease) :
	NodeResource(_autoRelease),
	src(_src),
	width(_width),
	height(_height),
	data(nullptr),
	channels(nullptr)
{
	load();
}

Texture::~Texture(){
	delete channels;
	delete data;
}

void Texture::load(){
	if(!loaded){
		if(channels == nullptr){
			channels = new int(0);
		}

		glGenTextures(1, &textureId);
				GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindTexture(GL_TEXTURE_2D, textureId);
				GLUtils::checkForError(0,__FILE__,__LINE__);

		unsigned char * tempData;

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
			throw "Invalid number of image channels";
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		loaded = true;
	}
}

void Texture::unload(){
	if(loaded){
		glDeleteTextures(1, &textureId);
		textureId = 0;
		loaded = false;
	}
}