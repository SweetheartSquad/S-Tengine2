#include "RenderSystem.h"
ShaderInterface *shader; 
RenderSystem::RenderSystem(){
	shaderArray = new std::vector<ShaderInterface*>;
	glfwWindow = glfwGetCurrentContext();
}

RenderSystem::~RenderSystem(void){
	delete shaderArray->at(0);
	delete shaderArray;
}

void RenderSystem::destroyRenderSystem(){
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
void RenderSystem::render(std::vector<Entity*> *renderChildren, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	ratio = width / static_cast<float>(height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glLoadIdentity();

	std::vector<Entity*>::iterator it = renderChildren->begin();
	while(it!=renderChildren->end()){
		GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindBuffer(GL_ARRAY_BUFFER, (*it)->vertexBuffer->getVertexBufferId());
			GLUtils::checkForError(0,__FILE__,__LINE__);
		(*it)->draw(projectionMatrix, viewMatrix);
			GLUtils::checkForError(0,__FILE__,__LINE__);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		++it;
	}

	glfwSwapBuffers(glfwWindow);
	GLUtils::checkForError(0,__FILE__,__LINE__);
}
 