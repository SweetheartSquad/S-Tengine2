#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "node/NodeResource.h"
#include "node/NodeLoadable.h"

class Texture : public virtual NodeResource{
public:
	Texture(const char* _src, int _width, int _height, bool _storeData, bool _autoRelease);
	~Texture();

	/**Source file location */
	const char* src;
	/**Texture Width */
	int width;
	/**Texture Height */
	int height;
	/**OpenGL Texture ID */
	GLuint textureId;
	/** Whether or not to store the image data in memory
	If you we are going to be re biniding the data a lot this may be useful*/
	bool storeData;
	/**The image data */
	unsigned char* data;
	/** Num channels */
	int* channels;
	/**Initializes the texture. Can be called to recreate the texture
	if the context is destroyed*/
	void load() override;
	/**Destroy the texture objects relation to the opengl context */
	void unload() override;
};