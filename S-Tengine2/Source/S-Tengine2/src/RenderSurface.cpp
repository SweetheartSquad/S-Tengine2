#pragma once

#include "RenderSurface.h"

#include "shader/Shader.h"

RenderSurface::RenderSurface(Shader * _shader):
	shader(_shader),
	scaleModeMag(GL_LINEAR),
	scaleModeMin(GL_LINEAR),
	dirty(false)
{
	vertices.push_back(Vertex(
		-1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 1
		));
	vertices.push_back(Vertex(
		1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 1
		));
	vertices.push_back(Vertex(
		1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 0
		));
	vertices.push_back(Vertex(
		-1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 0
		));

	load();
	clean();
}

RenderSurface::~RenderSurface(){
	unload();
	shader->decrementAndDelete();
}

void RenderSurface::load(){
	if(!loaded){
		shader->load();
		glUseProgram(shader->getProgramId());
		// Vertex Array Object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// Vertex Auffer Object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		GLUtils::configureVertexAttributes(shader->get_aVertexPosition(), 3, 0, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexColor(), 4, sizeof(float) * 3, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId, sizeof(Vertex));
		glBindVertexArray(0);
	}
	
	NodeLoadable::load();
}

void RenderSurface::unload(){
	if(loaded){
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
		vboId = 0;
		vaoId = 0;
		shader->unload();
	}
	NodeLoadable::unload();
}

void RenderSurface::render(GLuint _textureId, GLint _renderTo){

	glUseProgram(shader->getProgramId());
	glBindFramebuffer(GL_FRAMEBUFFER, _renderTo);
	glBindVertexArray(vaoId);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	
	// Texture repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Texture scaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin);

	clean();

	glDrawArrays(GL_QUADS, 0, vertices.size());
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
}

void RenderSurface::clean(){
	if (dirty){
		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices.size()), vertices.data(), GL_STATIC_DRAW);
		checkForGlError(0, __FILE__, __LINE__);
		dirty = false;
	}
}