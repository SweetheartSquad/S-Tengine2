#pragma once 

#include <gl/glew.h>
#include "Shader.h"

class FrameBufferInterface : public NodeResource{
public:

	GLuint frameBufferId;
	GLuint textureBufferId;
	GLuint renderBufferId;

	unsigned long int width;
	unsigned long int height;

	FrameBufferInterface(unsigned long int _width, unsigned long int _height, bool _autoRelase);
	~FrameBufferInterface();

	/**Initializes the frame buffer, texture buffer and render buffer*/
	void load() override;
	/**Destroys the frame buffer, texture buffer and render buffere*/
	void unload() override;
	/**Simply calls unload, reload*/
	void reload();
	/**Alters the buffers width and height, calling reload if they've changed*/
	void resize(unsigned long int _width, unsigned long int _height);
	/**Binds this frame buffer as the current in opengl*/
	void bindFrameBuffer();
	/**Prints a string representing the status of the framebuffer*/
	GLenum checkFrameBufferStatus();

};