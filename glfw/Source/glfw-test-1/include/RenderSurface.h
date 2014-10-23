#pragma once

#include<GL/glew.h>
#include <vector>

#include"FrameBufferVertex.h"
#include"FrameBufferInterface.h"

class RenderSurface{
private:
	std::vector<FrameBufferVertex> vertices;
public:
	RenderSurface(Shader * _shader);
	~RenderSurface();

	GLuint textureId;
	/** ID of the vertex array object */
	GLuint vaoId;
	/** ID of the vertex buffer object */
	GLuint vboId;
	/** ID of the index buffer object */
	GLuint iboId;

	Shader * shader;

	void load();
	void unload();
	void render(FrameBufferInterface _frameBuffer);
	void RenderSurface::configureVertexAttributes(GLint _vertexHandle, unsigned long int _arity, int _bufferOffset);
};