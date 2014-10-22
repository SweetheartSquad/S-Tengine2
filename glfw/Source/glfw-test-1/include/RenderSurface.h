#pragma once

#include<GL/glew.h>

class RenderSurface{
public:
	RenderSurface();
	~RenderSurface();

	GLuint textureId;
	
	void load();
	void unload();
};