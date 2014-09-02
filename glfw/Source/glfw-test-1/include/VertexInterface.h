#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>

#include <vector>

#include "Vox.h"
#include "ShaderInterface.h"
#include "Vertex.h"
#include "GLUtils.h"

class VertexInterface{
public:
	bool loaded;
	bool dirty;
	std::vector<Vertex>*vertices;
	std::vector<GLubyte>*indices;
public:
	GLuint iboId;
	GLuint vaoId;
	GLuint vboId;
	GLenum drawMode;
	GLsizei getVertCount();
	GLsizei getStride();
	GLenum polygonalDrawMode;
	
	VertexInterface(GLenum polygonalDrawMode, GLenum drawMode);
	~VertexInterface(void);
	
	//if unloaded, generates the VAO, VBO, IBO and flags as loaded
	void load();
	//if dirty, copies data from vertices and indices to VBO and IBO and flags as clean
	void clean();
	void render(ShaderInterface *shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset);
};

