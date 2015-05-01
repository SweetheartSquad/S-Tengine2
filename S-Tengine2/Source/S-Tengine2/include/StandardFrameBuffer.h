#pragma once

#include "FrameBufferInterface.h"

/*****************************************************************
*
* A FrameBuffer Interface with tow preconfigured channels
* Once channel is a texture channel which uses GL_RGBA as its format
* The other channel is a render buffer channel which uses GL_DEPTH_COMPONENT
* as its format.
*
******************************************************************/
class StandardFrameBuffer : public FrameBufferInterface{
public:
	explicit StandardFrameBuffer(bool _autoRelase);
	virtual ~StandardFrameBuffer();

	/**
	* @returns The openGL texture ID of the texture channel
	*/
	GLuint getTextureId();

	/**
	* @returns The openGL render buffer ID of the depth compoent buffer
	*/
	GLuint getRenderBufferId();
};