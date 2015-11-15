#pragma once

#include <node/NodeResource.h>

#include <GL/glew.h>

#include <string>
#include <ostream>
#include <iostream>

class FrameBufferInterface;

class Texture : public virtual NodeResource{
public:
	
	explicit Texture(std::string _src, bool _storeData, bool _autoRelease, bool _useMipmaps = true);
	explicit Texture(FrameBufferInterface * _frameBuffer, bool _storeData, int _frameBufferChannelIndex, int _channels, bool _autoRelease, bool _useMipmaps = true);
	~Texture();

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
	void bufferData();
	// creates the OpenGL texture and buffers it with the image data
	// also generates mipmaps if needed
	void bufferDataFirst();

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
	virtual void saveImageData(std::string _filename);

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
};