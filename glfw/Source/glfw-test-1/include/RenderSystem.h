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

	void render(std::vector<Entity*> *renderChildren, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
};

