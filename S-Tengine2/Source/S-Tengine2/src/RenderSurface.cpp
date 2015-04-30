#pragma once

#include "RenderSurface.h"

#include "shader/Shader.h"

RenderSurface::RenderSurface(Shader * _shader) :
	//MeshInterface(GL_QUADS, GL_STATIC_DRAW),
	NodeResource(false),
	shader2(_shader)
{
	scaleModeMag2 = GL_LINEAR;
	scaleModeMin2 = GL_LINEAR;
	vertices2.push_back(Vertex(
		-1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 1
		));
	vertices2.push_back(Vertex(
		1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 1
		));
	vertices2.push_back(Vertex(
		1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 0
		));
	vertices2.push_back(Vertex(
		-1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 0
		));
	indices2.push_back(0);
	indices2.push_back(1);
	indices2.push_back(2);
	indices2.push_back(3);

	dirty2 = true;

	load();
	clean();
}

RenderSurface::~RenderSurface(){
	unload();
	shader2->decrementAndDelete();
}

void RenderSurface::load(){
	if(!loaded){
		shader2->load();
		glUseProgram(shader2->getProgramId());
		// Vertex Array Object (VAO)
		glGenVertexArrays(1, &vaoId2);
		glBindVertexArray(vaoId2);

		// Vertex Buffer Object (VBO)
		glGenBuffers(1, &vboId2);
		glBindBuffer(GL_ARRAY_BUFFER, vboId2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices2.size(), vertices2.data(), GL_STATIC_DRAW);

		// Index Buffer Object (IBO)
		glGenBuffers(1, &iboId2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices2.size(), indices2.data(), GL_STATIC_DRAW);

		checkForGlError(0, __FILE__, __LINE__);
		GLUtils::configureVertexAttributes(shader2->get_aVertexPosition(), 3, 0, vaoId2, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader2->get_aVertexColor(), 4, sizeof(float) * 3, vaoId2, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader2->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId2, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader2->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId2, sizeof(Vertex));
		glBindVertexArray(0);
		checkForGlError(0, __FILE__, __LINE__);
	}
	
	NodeLoadable::load();
}

void RenderSurface::unload(){
	if (loaded){
		glDeleteBuffers(1, &iboId2);
		glDeleteBuffers(1, &vboId2);
		glDeleteVertexArrays(1, &vaoId2);
		vboId2 = 0;
		vaoId2 = 0;
		shader2->unload();
		checkForGlError(0, __FILE__, __LINE__);
	}
	NodeLoadable::unload();
}

void RenderSurface::render(GLuint _textureId, GLint _renderTo){

	if (glIsVertexArray(vaoId2) == GL_TRUE){
		if (glIsBuffer(vboId2) == GL_TRUE){
			if (glIsBuffer(iboId2) == GL_TRUE){

	glUseProgram(shader2->getProgramId());
	glBindFramebuffer(GL_FRAMEBUFFER, _renderTo);
	glBindVertexArray(vaoId2);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	// Texture repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Texture scaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin2);

	clean();

	//glDrawArrays(GL_QUADS, 0, vertices2.size());
	// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
	glDrawRangeElements(GL_QUADS, 0, indices2.size(), indices2.size(), GL_UNSIGNED_INT, 0);
	
	checkForGlError(0, __FILE__, __LINE__);
	glEnable(GL_DEPTH_TEST);
	checkForGlError(0, __FILE__, __LINE__);
	glBindVertexArray(0);
	checkForGlError(0, __FILE__, __LINE__);
			}
			else{
				std::cout << "ibo bad" << std::endl << std::endl;
			}
		}
		else{
			std::cout << "vbo bad" << std::endl << std::endl;
		}
	}
	else{
		std::cout << "vao bad" << std::endl << std::endl;
	}
}

void RenderSurface::clean(){
	if (dirty2){
		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices2.size()), vertices2.data(), GL_STATIC_DRAW);
		checkForGlError(0, __FILE__, __LINE__);

		// Index Buffer Object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (indices2.size()), indices2.data(), GL_STATIC_DRAW);
		checkForGlError(0, __FILE__, __LINE__);
		dirty2 = false;
	}
}