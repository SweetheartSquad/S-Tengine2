#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <Texture.h>
#include <Resource.h>
#include <GLUtils.h>
#include <Log.h>
#include <FrameBufferInterface.h>

Texture::Texture(std::string _src, bool _storeData, bool _autoRelease, bool _useMipmaps) :
	NodeResource(_autoRelease),
	src(_src),
	width(0),
	height(0),
	numPixels(0),
	numBytes(0),
	data(nullptr),
	channels(0),
	storeData(_storeData),
	useMipmaps(_useMipmaps)
{
}

Texture::Texture(FrameBufferInterface * _frameBuffer, bool _storeData, int _frameBufferChannelIndex, int _channels, bool _autoRelease, bool _useMipmaps) :
	NodeResource(_autoRelease),
	width(_frameBuffer->width),
	height(_frameBuffer->height),
	numPixels(width * height),
	numBytes(numPixels * channels),
	data(_frameBuffer->getPixelData(_frameBufferChannelIndex)),
	channels(_channels),
	storeData(_storeData),
	useMipmaps(_useMipmaps)
{
}

Texture::~Texture(){
	unload();
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
		ST_LOG_INFO("Unloading texture: " + this->src);
		glDeleteTextures(1, &textureId);
		textureId = 0;
		checkForGlError(0,__FILE__,__LINE__);
	}
	NodeLoadable::unload();
}

void Texture::loadImageData(){
	data = stbi_load(src.c_str(), &width, &height, &channels, 0);
	assert(data != nullptr);
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
	if(stbi_write_tga(ss.str().c_str(), width, height, channels, data, 1)){
		Log::info("Texture \"data/images/"+_filename+"\" saved");
	}else{
		Log::error("Texture \"data/images/"+_filename+"\" not saved");
	}
}

void Texture::bufferData(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkForGlError(0,__FILE__,__LINE__);
	GLenum format;
	switch(channels){
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: throw "Invalid number of image channels";
	}
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
	checkForGlError(0,__FILE__,__LINE__);
	
	if (useMipmaps){
		glGenerateMipmap(GL_TEXTURE_2D);
		checkForGlError(0,__FILE__,__LINE__);
	}
}

void Texture::bufferDataFirst(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkForGlError(0,__FILE__,__LINE__);
	GLenum format;
	switch(channels){
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: throw "Invalid number of image channels";
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	checkForGlError(0,__FILE__,__LINE__);
	
	if(useMipmaps){
		glGenerateMipmap(GL_TEXTURE_2D);
		checkForGlError(0,__FILE__,__LINE__);
	}
}