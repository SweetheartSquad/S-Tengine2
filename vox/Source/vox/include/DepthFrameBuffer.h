#pragma once

#include "FrameBufferInterface.h"

/*****************************************************************
*
* A FrameBuffer Interface with one preconfigured texture channel
* This channel uses DEPTH_COMPONENT as its internal format
*
******************************************************************/
class DepthFrameBuffer : public FrameBufferInterface{
public:

	/**
	* @param _autoRelease Whether or not to call the destructor on this object 
	*                     when nothing is referencing it 
	*/
	explicit DepthFrameBuffer(bool _autoRelease);
	~DepthFrameBuffer();

	/**
	* Gets the openGL texture from this buffer's texture channel
	*
	* @return The texture ID for the texture channel
	*/
	GLuint getTextureId();
};