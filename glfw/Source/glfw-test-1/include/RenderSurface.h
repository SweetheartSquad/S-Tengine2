#pragma once

#include<GL/glew.h>
#include <vector>

#include"FrameBufferVertex.h"
#include"FrameBufferInterface.h"

class Shader;

/******************************************************
*
* A surface to render a texture to. This class will most
* likely be used to render a frame buffer
*
******************************************************/
class RenderSurface{
private:
	/** The surface verties */
	std::vector<FrameBufferVertex> vertices;
public:

	/**
	* @param The shader to be used when rendering the surface
	*/
	explicit RenderSurface(Shader * _shader);
	~RenderSurface();
	/** ID of the vertex array object */
	GLuint vaoId;
	/** ID of the vertex buffer object */
	GLuint vboId;
	/**Shader to use when rendering the 2D surface*/
	Shader * shader;

	/**
	* Intializes the render surface quad along with the vertex array object
	* and vertex buffer object. The shader is also loaded if it is unloaded
	*/
	void load();

	/**
	* Unbinds the vertex array object and vertex buffer object.
	* Also unloads the shader id it is loaded
	*/
	void unload();

	/**
	* Renders a frame buffers texture buffer to this surface using the
	* specified frame buffer ID. The default is 0 which is the main openGL frame buffer
	* 
	* @param _textureId The openGL ID of the texture to render
	* @param _renderTo The ID of the framebuffer to render to
	*/
	void render(GLuint _textureId, GLint _renderTo = 0);
};