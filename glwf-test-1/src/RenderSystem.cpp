#include "RenderSystem.h"
ShaderInterface *shader; 
RenderSystem::RenderSystem(){
	GLUtils::checkForError(true,__FILE__,__LINE__);
	shaderArray = new std::vector<ShaderInterface*>;
	
	GLUtils::checkForError(true,__FILE__,__LINE__);
	shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	
	GLUtils::checkForError(true,__FILE__,__LINE__);
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

void RenderSystem::render(std::vector<Entity*> *renderChildren)
{
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	float ratio;
	int width, height;
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	ratio = width / static_cast<float>(height);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glViewport(0, 0, width, height);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glMatrixMode(GL_PROJECTION);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glLoadIdentity();
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glOrtho(-ratio, ratio, -1, 1, -1, 100);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glMatrixMode(GL_MODELVIEW);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glLoadIdentity();
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glRotatef(static_cast<float>(glfwGetTime()) * 50.f, 0.f, 1.f, 0.f);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glUseProgram(shader->getProgramId());
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glUniform4f(glGetUniformLocation(shader->getProgramId(), "uColor"),0.f,1.f,0.f,1.f);
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	std::vector<Entity*>::iterator it = renderChildren->begin();
	while(it!=renderChildren->end()){
		glPushMatrix();
		glBindBuffer(GL_ARRAY_BUFFER, (*it)->vertexBuffer->getVertexBufferId());
		(*it)->draw();
		(*it)->vertexBuffer->configureVertexAttributes(shader->get_aPositionVertex());
		(*it)->vertexBuffer->renderVertexBuffer();
		glPopMatrix();
		it++;
	}
	//GLUtils::checkForError(0,__FILE__,__LINE__);
	glfwSwapBuffers(glfwWindow);
	///GLUtils::checkForError(0,__FILE__,__LINE__);
	glfwPollEvents();
	//GLUtils::checkForError(0,__FILE__,__LINE__);
}
 