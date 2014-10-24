#pragma once 

#include <gl/glew.h>
#include "Shader.h"

class FrameBufferInterface{
public:

	GLuint frameBufferId;
	GLuint textureBufferId;
	GLuint renderBufferId;

	unsigned long int width;
	unsigned long int height;

	FrameBufferInterface(unsigned long int _width, unsigned long int _height);
	~FrameBufferInterface();

	void load();
	void unload();

	void resize(unsigned long int _width, unsigned long int _height);
	void bindFrameBuffer();

	GLenum checkFrameBufferStatus();

};