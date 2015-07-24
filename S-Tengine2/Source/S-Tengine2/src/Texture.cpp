#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Texture.h"
#include <Resource.h>
#include <GLUtils.h>
#include <Log.h>

Texture::Texture(std::string _src, bool _storeData, bool _autoRelease) :
	NodeResource(_autoRelease),
	src(_src),
	width(0),
	height(0),
	data(nullptr),
	channels(0),
	storeData(_storeData)
{
}

Texture::Texture(bool _storeData, bool _autoRelease) :
	NodeResource(_autoRelease),
	storeData(_storeData),
	data(nullptr),
	channels(0),
	width(0),
	height(0)
{	
}

Texture::~Texture(){
	unloadImageData();
}

void Texture::load(){
	if(!loaded){
		// load texture data if necessary
		if(data == nullptr){
			loadImageData();
		}

		// create and buffer OpenGL texture
		glGenTextures(1, &textureId);
		checkForGlError(0,__FILE__,__LINE__);
		bufferDataFirst();

		// delete texture data if necessary
		if(!storeData){
			unloadImageData();
		}
	}
	
	NodeLoadable::load();
}

void Texture::unload(){
	if(loaded){
		Log::info("Unloading texture: " + this->src);
		glDeleteTextures(1, &textureId);
		textureId = 0;
		checkForGlError(0,__FILE__,__LINE__);
	}
	NodeLoadable::unload();
}

void Texture::loadImageData(){
	data = stbi_load(src.c_str(), &width, &height, &channels, 0);
	numPixels = width * height;
	numBytes = numPixels * channels;
}

void Texture::unloadImageData(){
	free(data);
	data = nullptr;
}

void Texture::saveImageData(std::string _filename){
	std::stringstream ss;
	ss << "data/images/" << _filename;
	if(stbi_write_tga(ss.str().c_str(), width, height, channels, data)){
		Log::info("Texture \"data/images/"+_filename+"\" saved");
	}else{
		Log::error("Texture \"data/images/"+_filename+"\" not saved");
	}
}

void Texture::bufferData(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkForGlError(0,__FILE__,__LINE__);
	if(channels == 2) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_R8, GL_UNSIGNED_BYTE, data);
	}else if(channels == 3){
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}else if(channels == 4){
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}else{
		throw "Invalid number of image channels";
	}
	checkForGlError(0,__FILE__,__LINE__);
		
	glGenerateMipmap(GL_TEXTURE_2D); // is this call necessary?
	checkForGlError(0,__FILE__,__LINE__);
}

void Texture::bufferDataFirst(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkForGlError(0,__FILE__,__LINE__);
	if(channels == 2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}else if(channels == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}else if(channels == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}else{
		throw "Invalid number of image channels";
	}
	checkForGlError(0,__FILE__,__LINE__);
		
	glGenerateMipmap(GL_TEXTURE_2D);
	checkForGlError(0,__FILE__,__LINE__);
}