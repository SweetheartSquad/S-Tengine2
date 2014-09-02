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
	glEnable(GL_DEPTH_TEST);
		GLUtils::checkForError(0,__FILE__,__LINE__);

	//Back-face culling
	/*glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CW); // GL_CCW for counter clock-wise
		GLUtils::checkForError(0,__FILE__,__LINE__);*/

	//glLoadIdentity();
	//	GLUtils::checkForError(0,__FILE__,__LINE__);


		for(Entity * e : *renderChildren){
			e->draw(projectionMatrix, viewMatrix);
		}

	/*std::vector<Entity*>::iterator it = renderChildren->begin();
	while(it!=renderChildren->end()){

		
	//bind VAO, VBO, IBO
	/*glBindVertexArray((*it)->vertexInterface->vaoId);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glBindBuffer(GL_ARRAY_BUFFER, (*it)->vertexInterface->vboId);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->vertexInterface->iboId);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	//update verts in VAO/VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*((*it)->vertices->size()), (*it)->vertices->data(), GL_STATIC_DRAW);*/
/*
		(*it)->draw(projectionMatrix, viewMatrix);
		++it;
			GLUtils::checkForError(0,__FILE__,__LINE__);

			for(unsigned long int i = 0; i < (*it)->children->size(); ++i){
				(*it)->children->at(i)->draw(projectionMatrix, viewMatrix);
			}
	}
	*/
	glfwSwapBuffers(vox::currentContext);
		GLUtils::checkForError(0,__FILE__,__LINE__);
}
 