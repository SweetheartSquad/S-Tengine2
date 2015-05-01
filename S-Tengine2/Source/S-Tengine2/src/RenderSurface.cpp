#pragma once

#include "RenderSurface.h"

#include "shader/Shader.h"

RenderSurface::RenderSurface(Shader * _shader) :
	MeshInterface(GL_QUADS, GL_STATIC_DRAW),
	NodeResource(true),
	shader(_shader)
{
	pushVert(Vertex(
		-1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 1
	));
	pushVert(Vertex(
		1.f, 1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 1
	));
	pushVert(Vertex(
		1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		1, 0
	));
	pushVert(Vertex(
		-1.f, -1.f, 0.f,
		1, 1, 1, 1,
		0, 0, 1,
		0, 0
	));

	loaded = false;
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

		// Vertex Buffer Object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), drawMode);

		// Index Buffer Object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), drawMode);

		checkForGlError(0, __FILE__, __LINE__);
		GLUtils::configureVertexAttributes(shader->get_aVertexPosition(), 3, 0, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexColor(), 4, sizeof(float) * 3, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId, sizeof(Vertex));
		GLUtils::configureVertexAttributes(shader->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId, sizeof(Vertex));
		glBindVertexArray(0);
		checkForGlError(0, __FILE__, __LINE__);
	}
	
	NodeLoadable::load();
}

void RenderSurface::unload(){
	if (loaded){
		glDeleteBuffers(1, &iboId);
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
		vboId = 0;
		vaoId = 0;
		shader->unload();
		checkForGlError(0, __FILE__, __LINE__);
	}
	NodeLoadable::unload();
}

void RenderSurface::render(GLuint _textureId, GLint _renderTo){
	if (glIsVertexArray(vaoId) == GL_TRUE){
		if (glIsBuffer(vboId) == GL_TRUE){
			if (glIsBuffer(iboId) == GL_TRUE){
				glUseProgram(shader->getProgramId());
				glBindFramebuffer(GL_FRAMEBUFFER, _renderTo);
				glBindVertexArray(vaoId);

				GLboolean dt = glIsEnabled(GL_DEPTH_TEST);
				if (dt == GL_TRUE){
					glDisable(GL_DEPTH_TEST);
				}
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _textureId);
	
				// Texture repeat
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uvEdgeMode);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uvEdgeMode);
	
				// Texture scaling mode
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin);

				clean();

				//glDrawArrays(GL_QUADS, 0, vertices2.size());
				// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
				glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);

				if (dt == GL_TRUE){
					glEnable(GL_DEPTH_TEST);
				}
				glBindVertexArray(0);

				checkForGlError(0, __FILE__, __LINE__);
			}else{
				std::cout << "ibo bad" << std::endl << std::endl;
			}
		}else{
			std::cout << "vbo bad" << std::endl << std::endl;
		}
	}else{
		std::cout << "vao bad " << std::endl << std::endl;
	}
}