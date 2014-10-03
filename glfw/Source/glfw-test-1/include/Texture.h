#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "ReferenceManager.h"

class Texture : public ReferenceManager
{
public:
	Texture(const char* _src, int _width, int _height, bool _storeData);
	~Texture();

	//Source file location
	const char* src;
	//Texture Width
	int width;
	//Texture Height
	int height;
	//OpenGL Texture ID
	GLuint textureId;
	/*Whether or not to store the image data in memory
	If you we are going to be re biniding the data a lot this may be useful*/
	bool storeData;
	//The image data
	unsigned char* data;
	// Num channels
	int* channels;
	/*Initializes the texture. Can be called to recreate the texture
	if the context is destroyed*/
	void load();
	//Destroy the texture objects relation to the opengl context
	void unload();
};