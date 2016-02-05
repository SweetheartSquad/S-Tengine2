#pragma once

#include <CubeMapTexture.h>
#include <GLUtils.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

CubeMapTexture::CubeMapTexture(std::string _srcPrefix, std::string _srcSuffix, bool _storeData, bool _autoRelease, bool _useMipmaps) :
	Texture(_srcPrefix + _srcSuffix, _storeData, _autoRelease, _useMipmaps),
	NodeResource(_autoRelease)
{
	faceSrc[0] = _srcPrefix + "/posx." + _srcSuffix;
	faceSrc[1] = _srcPrefix + "/negx." + _srcSuffix;
	faceSrc[2] = _srcPrefix + "/posy." + _srcSuffix;
	faceSrc[3] = _srcPrefix + "/negy." + _srcSuffix;
	faceSrc[4] = _srcPrefix + "/posz." + _srcSuffix;
	faceSrc[5] = _srcPrefix + "/negz." + _srcSuffix;
}

void CubeMapTexture::bufferData() const{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	checkForGlError(false);
	GLenum format;
	switch(channels){
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: throw "Invalid number of image channels";
	}
	for(unsigned long int i = 0; i < 6; ++i){
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, faceData[i]);
		checkForGlError(false);
	}
	
	if (useMipmaps){
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		checkForGlError(false);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMapTexture::bufferDataFirst() const{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	checkForGlError(false);
	GLenum format;
	switch(channels){
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: throw "Invalid number of image channels";
	}
	for(unsigned long int i = 0; i < 6; ++i){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, faceData[i]);
		checkForGlError(false);
	}
	
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if(useMipmaps){
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		checkForGlError(false);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMapTexture::loadImageData(){
	for(unsigned long int i = 0; i < 6; ++i){
		faceData[i] = stbi_load(faceSrc[i].c_str(), &width, &height, &channels, 0);
		assert(faceData[i] != nullptr);
		numPixels = width * height;
		numBytes = numPixels * channels;
	}
}

void CubeMapTexture::freeImageData(){
	for(unsigned long int i = 0; i < 6; ++i){
		free(faceData[i]);
		faceData[i] = nullptr;
	}
}