#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "node/NodeResource.h"
#include "node/NodeLoadable.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <string>
#include <ostream>

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
	/** Num channels (can't be unsigned long because of SOIL) */
	int channels;
	// Pixels in texture (width * height)
	unsigned long int numPixels;
	// Bytes in texture (numPixels * channels)
	unsigned long int numBytes;
	/**OpenGL Texture ID */
	GLuint textureId;
	/** Whether or not to store the image data in memory
	If you we are going to be re biniding the data a lot this may be useful*/
	bool storeData;
	/**The image data */
	unsigned char * data;

	
	// updates the OpenGL texture with the image data (assumes texture exists already)
	// also generates mipmaps
	void bufferData();
	// creates the OpenGL texture and buffers it with the image data
	// also generates mipmaps
	void bufferDataFirst();

	/**Initializes the texture. Can be called to recreate the texture
	if the context is destroyed*/
	virtual void load() override;
	/**Destroy the texture objects relation to the opengl context */
	virtual void unload() override;

	virtual void loadImageData();

	friend std::ostream& operator<<(std::ostream& os, const Texture& obj){
		return os
			<< static_cast<const NodeResource&>(obj)
			<< " src: " << obj.src
			<< " width: " << obj.width
			<< " height: " << obj.height
			<< " textureId: " << obj.textureId
			<< " storeData: " << obj.storeData
			<< " data: " << obj.data
			<< " channels: " << obj.channels;
	}
};