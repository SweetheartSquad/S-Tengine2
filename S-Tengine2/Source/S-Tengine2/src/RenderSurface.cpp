#pragma once

#include "RenderSurface.h"

#include "shader/Shader.h"

RenderSurface::RenderSurface(Shader * _shader):
	shader(_shader),
	scaleModeMag(GL_LINEAR),
	scaleModeMin(GL_LINEAR),
	dirty(false)
{
	vertices.push_back(FrameBufferVertex(-1.0,  1.0, 0.0, 1.0));
	vertices.push_back(FrameBufferVertex( 1.0,  1.0, 1.0, 1.0));
	vertices.push_back(FrameBufferVertex( 1.0, -1.0, 1.0, 0.0));
	vertices.push_back(FrameBufferVertex(-1.0, -1.0, 0.0, 0.0));

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(FrameBufferVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		GLint aVertexPosition	= shader->get_aVertexPosition();
		GLint aVertexUvs		= shader->get_aVertexUVs();

		GLUtils::configureVertexAttributes(aVertexPosition, 2, 0, vaoId, sizeof(FrameBufferVertex));
		GLUtils::configureVertexAttributes(aVertexUvs, 2, sizeof(float) * 2, vaoId, sizeof(FrameBufferVertex));
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(FrameBufferVertex) * (vertices.size()), vertices.data(), GL_STATIC_DRAW);
		checkForGlError(0, __FILE__, __LINE__);
		dirty = false;
	}
}