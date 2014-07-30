#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "ShaderInterface.h"
#include <vector>

class RenderSystem
{
private:
	RenderSystem(void);
	~RenderSystem(void);

	GLFWwindow *glfwWindow;
	std::vector<ShaderInterface *> *shaderArray;

public:
	static RenderSystem& getInstance();
	static void destroyRenderSystem();

	void render(VertexBuffer *vertexBuffer);
};

