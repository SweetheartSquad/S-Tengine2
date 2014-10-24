#pragma once

#include<GL/glew.h>
#include <vector>

#include"FrameBufferVertex.h"
#include"FrameBufferInterface.h"

class RenderSurface{
private:
	std::vector<FrameBufferVertex> vertices;

public:
	explicit RenderSurface(Shader * _shader);
	~RenderSurface();

	GLuint textureId;
	/** ID of the vertex array object */
	GLuint vaoId;
	/** ID of the vertex buffer object */
	GLuint vboId;

	Shader * shader;

	void load();
	void unload();
	void render(FrameBufferInterface _frameBuffer);
};