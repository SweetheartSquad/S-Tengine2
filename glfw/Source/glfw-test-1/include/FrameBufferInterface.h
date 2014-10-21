#pragma once 

#include <gl/glew.h>

class FrameBufferInterface{
public:

	GLuint frameBufferId;
	GLuint textureBufferId;

	FrameBufferInterface();
	~FrameBufferInterface();

	void load();
	void unload();

	void bindTextureBuffer();
	void bindFrameBuffer();

	GLenum checkFrameBufferStatus();

};