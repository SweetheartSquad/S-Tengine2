#pragma once

#include "RenderSurface.h"

#include "Shader.h"

RenderSurface::RenderSurface(Shader * _shader):
	shader(_shader)
{
	vertices.push_back(FrameBufferVertex(-1.0,  1.0, 0.0, 1.0));
	vertices.push_back(FrameBufferVertex( 1.0,  1.0, 1.0, 1.0));
	vertices.push_back(FrameBufferVertex( 1.0, -1.0, 1.0, 0.0));
	vertices.push_back(FrameBufferVertex(-1.0, -1.0, 0.0, 0.0));

	load();
}

RenderSurface::~RenderSurface(){
	unload();
	shader->decrementAndDelete();
}

void RenderSurface::load(){
	if(!shader->loaded){
		shader->load();
	}
	glUseProgram(shader->getProgramId());
	// Vertex Array Object (VAO)
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// Vertex Auffer Object (VBO)
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FrameBufferVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	GLint aVertexPosition	= shader->get_aVertexPosition();
	GLint aVertexUvs		= shader->get_aVertexUVs();

	GLUtils::configureVertexAttributes(aVertexPosition, 2, 0, vaoId, sizeof(FrameBufferVertex));
	GLUtils::configureVertexAttributes(aVertexUvs, 2, sizeof(float) * 2, vaoId, sizeof(FrameBufferVertex));
	glBindVertexArray(0);
}

void RenderSurface::unload(){
	glDeleteBuffers(1, &vboId);
	glDeleteVertexArrays(1, &vaoId);
	vboId = 0;
	vaoId = 0;
	if(shader->loaded){
		shader->unload();
	}
}

void RenderSurface::render(GLuint _textureId, GLint _renderTo){
	glUseProgram(shader->getProgramId());
	glBindFramebuffer(GL_FRAMEBUFFER, _renderTo);
	glBindVertexArray(vaoId);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glDrawArrays(GL_QUADS, 0, vertices.size());
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
}