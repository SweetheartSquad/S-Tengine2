#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MeshInterface.h"
#include "ShaderInterface.h"
#include <vector>
#include "Entity.h"

#include "Vox.h"

class RenderSystem{
private:
	RenderSystem(void);
	~RenderSystem(void);

	GLFWwindow *glfwWindow;
	std::vector<ShaderInterface *> *shaderArray;

public:
	static RenderSystem& getInstance();
	static void destroyRenderSystem();

	void render(GLFWwindow * _context, std::vector<Entity*> *_renderChildren, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
};
