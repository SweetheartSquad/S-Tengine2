#pragma once

#include<GL/glew.h>
#include <vector>

#include"FrameBufferVertex.h"
#include"FrameBufferInterface.h"

class Shader;

class RenderSurface{
private:
	std::vector<FrameBufferVertex> vertices;

public:
	explicit RenderSurface(Shader * _shader);
	~RenderSurface();
	/** ID of the vertex array object */
	GLuint vaoId;
	/** ID of the vertex buffer object */
	GLuint vboId;
	/**Shader to use when rendering the 2D surface*/
	Shader * shader;
	/**Intializes the render surface quad*/
	void load();
	/**Destroys the render surface quad*/
	void unload();
	/**Calls unload, reload*/
	void reload();
	/**Renders a frame buffers texture buffer to this surface using the 
	*  Main openGL framne buffer */
	void render(GLuint _textureId);
};