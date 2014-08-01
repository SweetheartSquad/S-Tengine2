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
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	ratio = width / static_cast<float>(height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1, 1, -1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(static_cast<float>(glfwGetTime()) * 50.f, 0.f, 1.f, 0.f);
	glUseProgram(shader->getProgramId());
	glUniform4f(glGetUniformLocation(shader->getProgramId(), "uColor"),0.f,1.f,0.f,1.f);
	
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
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
}
 