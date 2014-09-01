#include "RenderSystem.h"
ShaderInterface *shader; 
RenderSystem::RenderSystem(){
	shaderArray = new std::vector<ShaderInterface*>;
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
void RenderSystem::render(std::vector<Entity*> *renderChildren, glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	float ratio;
	int width, height;
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glfwGetFramebufferSize(vox::currentContext, &width, &height);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	ratio = width / static_cast<float>(height);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glViewport(0, 0, width, height);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glCullFace(GL_FRONT);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glEnable(GL_DEPTH_TEST);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glCullFace(GL_FRONT);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	//glLoadIdentity();
		GLUtils::checkForError(0,__FILE__,__LINE__);

	std::vector<Entity*>::iterator it = renderChildren->begin();
	while(it!=renderChildren->end()){
		std::cout << "test" << std::endl;
			GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindBuffer(GL_ARRAY_BUFFER, (*it)->vertexInterface->getVertexInterfaceId());
			GLUtils::checkForError(0,__FILE__,__LINE__);
		(*it)->draw(projectionMatrix, viewMatrix);
			GLUtils::checkForError(0,__FILE__,__LINE__);
		++it;
	}

	glfwSwapBuffers(vox::currentContext);
		GLUtils::checkForError(0,__FILE__,__LINE__);
}
 