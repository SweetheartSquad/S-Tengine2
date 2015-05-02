#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "node/NodeResource.h"
#include "node/NodeLoadable.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class Texture : public virtual NodeResource{
public:
	Texture(std::string _src, unsigned long int _width, unsigned long int _height, bool _storeData, bool _autoRelease);
	Texture(bool _storeData, bool _autoRelease);
	~Texture();

	/**Source file location */
	std::string src;
	/**Texture Width */
	unsigned long int width;
	/**Texture Height */
	unsigned long int height;
	/**OpenGL Texture ID */
	GLuint textureId;
	/** Whether or not to store the image data in memory
	If you we are going to be re biniding the data a lot this may be useful*/
	bool storeData;
	/**The image data */
	unsigned char * data;
	/** Num channels (can't be unsigned long because of SOIL) */
	int channels;
	/**Initializes the texture. Can be called to recreate the texture
	if the context is destroyed*/
	virtual void load() override;
	/**Destroy the texture objects relation to the opengl context */
	virtual void unload() override;
};