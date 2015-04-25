#pragma once

#include "Texture.h"
#include <Resource.h>
#include <GLUtils.h>
#include <SOIL.h>

// maybe replace chars with strings

Texture::Texture(std::string _src, unsigned long int _width, unsigned long int _height, bool _storeData, bool _autoRelease) :
	NodeResource(_autoRelease),
	src(_src),
	width(_width),
	height(_height),
	data(nullptr),
	channels(nullptr)
{

}

Texture::Texture(FT_Bitmap _glyph, bool _storeDate, bool _autoRelease):
	NodeResource(_autoRelease),
	width(_glyph.width),
	height(_glyph.rows),
	storeData(_storeDate),
	data(_glyph.buffer),
	channels(new int(4))
{

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
		
				checkForGlError(0,__FILE__,__LINE__);
		glGenTextures(1, &textureId);
				checkForGlError(0,__FILE__,__LINE__);
		glBindTexture(GL_TEXTURE_2D, textureId);
				checkForGlError(0,__FILE__,__LINE__);

		unsigned char * tempData = nullptr;

		if(data != nullptr){
			tempData = data;
		}else{
			tempData = Resource::loadImage(src.c_str(), width, height, SOIL_LOAD_AUTO, channels);
			if(storeData){
				data = tempData;
			}
		}

		if(*channels == 3){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempData);
		}else if(*channels == 4){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, tempData);
		}else{
			throw "Invalid number of image channels";
		}
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	NodeLoadable::load();
}

void Texture::unload(){
	if(loaded){
		glDeleteTextures(1, &textureId);
		textureId = 0;
		checkForGlError(0,__FILE__,__LINE__);
		std::cout << loaded << " tex: " << this->src << std::endl;
	}
	
	NodeLoadable::unload();
}