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
	channels(0)
{

}

Texture::Texture(FT_Bitmap _glyph, bool _storeDate, bool _autoRelease):
	NodeResource(_autoRelease),
	width(_glyph.width),
	height(_glyph.rows),
	storeData(_storeDate),
	data(_glyph.buffer),
	channels(2)
{

}

Texture::~Texture(){
	delete data;
}

void Texture::load(){
	if(!loaded){
		glGenTextures(1, &textureId);
		checkForGlError(0,__FILE__,__LINE__);
		glBindTexture(GL_TEXTURE_2D, textureId);
		checkForGlError(0,__FILE__,__LINE__);

		//unsigned char * tempData = nullptr;
		if(data == nullptr){
			data = Resource::loadImage(src.c_str(), width, height, SOIL_LOAD_AUTO, &channels);
		}

		if(channels == 2) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		}else if(channels == 3){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}else if(channels == 4){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		}else{
			throw "Invalid number of image channels";
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);

		if(!storeData){
			data = nullptr;
		}
	}
	
	NodeLoadable::load();
}

void Texture::unload(){
	if(loaded){
		std::cout << " tex: " << this->src << std::endl;
		glDeleteTextures(1, &textureId);
		textureId = 0;
		checkForGlError(0,__FILE__,__LINE__);
	}
	NodeLoadable::unload();
}