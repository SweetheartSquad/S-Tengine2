#include "RenderSystem.h"
ShaderInterface *shader; 
RenderSystem::RenderSystem()
{
	shaderArray = new std::vector<ShaderInterface*>;
	shader = new ShaderInterface("ColourShader.vsh","ColourShader.fsh");
	glfwWindow = glfwGetCurrentContext();
}

RenderSystem::~RenderSystem(void)
{
	delete shaderArray->at(0);
	delete shaderArray;
}

void RenderSystem::destroyRenderSystem()
{
	RenderSystem *renderSystem = &getInstance();
	delete renderSystem;
}

RenderSystem& RenderSystem::getInstance(){
	static RenderSystem *renderSystem;
	if(renderSystem == 0){
		renderSystem = new RenderSystem();
	}
	return *renderSystem;
}

void RenderSystem::render(VertexBuffer *vertexBuffer)
{
	float ratio;
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    ratio = width / static_cast<float>(height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(static_cast<float>(glfwGetTime()) * 50.f, 0.f, 0.f, 1.f);
    glUseProgram(shader->getProgramId());
	//glUniform4f(0,0.f,1.f,0.f,1.f);

	//vertexBuffer->configureVertexAttributes(vertexBuffer->getVertexBufferId());
	vertexBuffer->renderVertexBuffer();

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}
 