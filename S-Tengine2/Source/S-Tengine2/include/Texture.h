#pragma once

#include <node/NodeResource.h>

#include <GL/glew.h>

#include <string>
#include <ostream>
#include <iostream>

class Texture : public virtual NodeResource{
public:
	
	explicit Texture(std::string _src, bool _storeData, bool _autoRelease, bool _useMipmaps = true);
	virtual ~Texture();

	/**Source file location */
	std::string src;
	/**Texture Width */
	int width;
	/**Texture Height */
	int height;
	/** Num channels (can't be unsigned long because of SOIL) */
	int channels;
	// Pixels in texture (width * height)
	unsigned long int numPixels;
	// Bytes in texture (numPixels * channels)
	unsigned long int numBytes;

	// sets the width, height, channels, and calculates the numPixels and numBytes
	void resize(int _width, int _height, int _channels);

	/**OpenGL Texture ID */
	GLuint textureId;
	/** Whether or not to store the image data in memory
	If you we are going to be re biniding the data a lot this may be useful*/
	bool storeData;
	/**The image data */
	unsigned char * data;
	// whether or not mipmaps will be generated after buffering data
	bool useMipmaps;

	
	// updates the OpenGL texture with the image data (assumes texture exists already)
	// also generates mipmaps if needed
	virtual void bufferData() const;
	// creates the OpenGL texture and buffers it with the image data
	// also generates mipmaps if needed
	virtual void bufferDataFirst() const;

	/**Initializes the texture. Can be called to recreate the texture
	if the context is destroyed*/
	virtual void load() override;
	/**Destroy the texture objects relation to the opengl context */
	virtual void unload() override;
	
	virtual void loadImageData();
	virtual void unloadImageData();

	// saves the texture data to _filename
	// assumes TGA output regardless of _filename
	// file path is relative to "data/images"
	virtual void saveImageData(const std::string & _filename, bool _reverse = true, bool _png = true);

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
	};

protected:
	virtual void genTextures();
};