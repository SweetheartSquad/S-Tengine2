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
	The texture magnification function is used when the pixel being textured maps to an area less than or equal to one texture element.
	i.e. Filter when scaling up textures

	GL_NEAREST: Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	GL_LINEAR: Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	*/
	GLenum scaleModeMag;

	/**
	The texture minifying function is used whenever the pixel being textured maps to an area greater than one texture element.
	i.e. Filter when scaling down textures

	GL_NEAREST: Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	GL_LINEAR: Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	GL_NEAREST_MIPMAP_NEAREST: Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_NEAREST criterion (the texture element nearest to the center of the pixel) to produce a texture value.
	GL_LINEAR_MIPMAP_NEAREST: Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_LINEAR criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value.
	GL_NEAREST_MIPMAP_LINEAR: Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_NEAREST criterion (the texture element nearest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
	GL_LINEAR_MIPMAP_LINEAR: Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_LINEAR criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
	*/
	GLenum scaleModeMin;



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

	// saves the contents of _fbochannel to _filename
	// assumes TGA output regardless of _filename
	// file path is relative to "data/images"
	void saveToFile(const char * _filename, unsigned long int _fbochannel);
};