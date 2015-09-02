
#pragma once

#include "RenderSurface.h"

#include "shader/Shader.h"

RenderSurface::RenderSurface(Shader * _shader) :
	MeshInterface(GL_QUADS, GL_STATIC_DRAW),
	NodeResource(true),
	NodeShadable(_shader)
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

		configureDefaultVertexAttributes(shader);
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

void RenderSurface::render(GLuint _textureId, GLint _renderTo, bool _disableBlending){
	load();
	clean();

	if (glIsVertexArray(vaoId) == GL_TRUE){
		if (glIsBuffer(vboId) == GL_TRUE){
			if (glIsBuffer(iboId) == GL_TRUE){
				glUseProgram(shader->getProgramId());
				glBindFramebuffer(GL_FRAMEBUFFER, _renderTo);
				glBindVertexArray(vaoId);
				checkForGlError(0,__FILE__,__LINE__);

				if(_disableBlending){
					glDisable(GL_BLEND);
				}

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

				// we might need to try and optimize this bit
				if(scaleModeMin == GL_NEAREST_MIPMAP_NEAREST
					|| scaleModeMin == GL_LINEAR_MIPMAP_NEAREST
					|| scaleModeMin == GL_NEAREST_MIPMAP_LINEAR
					|| scaleModeMin == GL_LINEAR_MIPMAP_LINEAR
					){
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
				glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);

				if (dt == GL_TRUE){
					glEnable(GL_DEPTH_TEST);
				}

				if(_disableBlending){
					glEnable(GL_BLEND);
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