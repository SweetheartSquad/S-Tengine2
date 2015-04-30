#pragma once

#include<GL/glew.h>
#include <vector>

#include "node\NodeLoadable.h"
#include <MeshInterface.h>

class Shader;

/******************************************************
*
* A surface to render a texture to. This class will most
* likely be used to render a frame buffer
*
******************************************************/
class RenderSurface : public NodeLoadable{
public:
	/** The surface vertices */
	std::vector<Vertex> vertices;
	bool dirty;

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
	* Intializes the render surface quad along with the vertex array object
	* and vertex buffer object. The shader is also loaded if it is unloaded
	*/
	void load() override;

	/**
	* Unbinds the vertex array object and vertex buffer object.
	* Also unloads the shader id it is loaded
	*/
	void unload() override;

	/**
	* Updates the vertex data
	*/
	void clean();

	/**
	* Renders a frame buffers texture buffer to this surface using the
	* specified frame buffer ID. The default is 0 which is the main openGL frame buffer
	* 
	* @param _textureId The openGL ID of the texture to render
	* @param _renderTo The ID of the framebuffer to render to
	*/
	void render(GLuint _textureId, GLint _renderTo = 0);
};