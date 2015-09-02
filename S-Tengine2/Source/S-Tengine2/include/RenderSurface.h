#pragma once

#include<GL/glew.h>
#include <vector>

#include "node\NodeLoadable.h"
#include "node\NodeResource.h"
#include <MeshInterface.h>
#include <node\NodeShadable.h>

class Shader;

/******************************************************
*
* A surface to render a texture to. This class will most
* likely be used to render a frame buffer
*
******************************************************/
class RenderSurface : public virtual MeshInterface, public virtual NodeShadable{
public:
	/**
	* @param The shader to be used when rendering the surface
	*/
	explicit RenderSurface(Shader * _shader);
	~RenderSurface();

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
	* Renders a frame buffers texture buffer to this surface using the
	* specified frame buffer ID. The default is 0 which is the main openGL frame buffer
	* 
	* @param _textureId The openGL ID of the texture to render
	* @param _renderTo The ID of the framebuffer to render to
	* @param _disableBlending Whether or not to disable glBlend
	*/
	void render(GLuint _textureId, GLint _renderTo = 0, bool _disableBlending = true);
};