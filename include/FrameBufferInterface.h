#pragma once

#include <gl/glew.h>
#include <vector>

#include "FrameBufferChannel.h"
#include "node/NodeResource.h"

/*******************************************************
*
* An interface for working with framebuffers in OpenGL
* This class has a a list of FrameBufferChannels. These channels describe
* the type of date for use in the frame buffer. The scene can be rendered to a frame 
* buffer by calling bindBuffer() followed by Scene::render()
*
********************************************************/
class FrameBufferInterface : public NodeResource{
public:

	//The openGL ID for the Frame Buffer
	GLuint frameBufferId;

	//The Frame buffer channels
	std::vector<FrameBufferChannel> frameBufferChannels;

	//The height of the frame buffer
	unsigned long int width;
	//The width of the frame buffer
	unsigned long int height;

	/**
	* @param _frameBufferChannels A vector containing the frame buffer channels to be used
	* @param _width The width of the frame buffer
	* @param _height The heigt of the framebuffer
	* @param _autoRelease Whether or not this should be deleted when all references to it are deleted
	*/
	FrameBufferInterface(std::vector<FrameBufferChannel> _frameBufferChannels, unsigned long int _width, unsigned long int _height, bool _autoRelase);
	~FrameBufferInterface();

	/**
	* Initializes the frame buffer, texture buffer and render buffer
	*/
	void load() override;
	
	/**
	* Destroys the frame buffer, texture buffer and render buffere
	*/
	void unload() override;
	
	/**
	* Simply calls unload, reload
	*/
	void reload();
	
	/**
	* Alters the buffers width and heights calling reload if they've changed
	*
	* @param _width The width to resize the framebuffer to
	* @param _height The height to resize the framebuffer to
 	*/
	void resize(unsigned long int _width, unsigned long int _height);
	
	/**
	* Binds this frame buffer as the current in opengl
	*/
	void bindFrameBuffer();
	
	/**
	* Prints a string representing the status of the framebuffer
	*/
	GLenum checkFrameBufferStatus();
};