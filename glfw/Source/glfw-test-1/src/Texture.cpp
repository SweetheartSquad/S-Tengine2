#include "Texture.h"

Texture::Texture(const char* _src, int _width, int _height, bool _storeData):
	src(_src),
	width(_width),
	height(_height),
	data(nullptr)
{
	init();
}

Texture::~Texture(){
}

void Texture::init(){
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	unsigned char* tempData;

	if(data != nullptr){
		tempData = data;
	}else{
		tempData = Resource::loadImage(src, width, height, SOIL_LOAD_RGB);
		if(storeData){
			data = tempData;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::unload(){
	glDeleteTextures(1, &textureId);
	textureId = 0;
}