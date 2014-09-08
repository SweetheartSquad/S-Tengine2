#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>

#include <vector>

#include "Vox.h"
#include "ShaderInterface.h"
#include "Vertex.h"
#include "GLUtils.h"


class MeshInterface{
public:
	bool loaded;	//whether the vao, vbo, and ibo have been generated and initialized
	bool dirty;		//whether the vbo and ibo contain up-to-date vertex and index data
	std::vector<Vertex> vertices; //vertex data for the vbo
	std::vector<GLubyte> indices; //index data for the ibo
public:
	GLuint vaoId;	//ID of the vertex array object
	GLuint vboId;	//ID of the vertex buffer object
	GLuint iboId;	//ID of the index buffer object
	
	/* OpenGL memory hint; possible values: 
		STATIC - The user will set the data once
		DYNAMIC - The user will set the data occasionally
		STREAM - The user will be changing the data after every use(or almost every use)
	*/
	GLenum drawMode;

	/* OpenGL draw mode; possible values:
		GL_POINTS - draws points (1, 2, 3, ...)
		GL_LINES - draws individual line segments (1-2, 3-4, 5-6, ...)
		GL_LINE_STRIP - draws connected line segments (1-2, 2-3, 3-4, ...)
		GL_LINE_LOOP - same as GL_LINE_STRIP, with an additional closing segment (1-2, 2-3, ..., n-1)
		GL_TRIANGLES - draws individual triangles (1-2-3, 4-5-6, 7-8-9, ...)
		GL_TRIANGLE_STRIP - draws triangle sequences (1-2-3, 2-3-4, 3-4-5, ...)
		GL_TRIANGLE_FAN - draws triangles around a central vertex (1-2-3, 1-3-4, 1-4-5, ...)
		GL_QUADS - draws individual qauds (1-2-3-4, 5-6-7-8, 9-10-11-12, ...)
		GL_QUAD_STRIP - draws quad sequences (1-2-4-3, 3-4-6-5, 5-6-8-7, ...)
		GL_POLYGON - draws single n-gon (1-2-3-...-n)
	*/
	GLenum polygonalDrawMode;

	//returns vertices.size()
	GLsizei getVertCount();
	//returns sizeof(Vertex)
	GLsizei getStride();
	
	MeshInterface(GLenum polygonalDrawMode, GLenum drawMode);
	~MeshInterface(void);
	
	//if unloaded, generates the VAO, VBO, IBO and flags as loaded
	void load();
	//if loaded, deletes the VAO, VBO, IBO and flags as not loaded and dirty
	void unload();
	//if dirty, copies data from vertices and indices to VBO and IBO and flags as clean
	void clean();
	//renders the vao using the given shader and model-view-projection
	void render(ShaderInterface *shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	//configures shader attributes (?)
	void configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset);
	//A helper method to configure all the starndard vertex attributes - Position, Colours, Normals
	void configureDefaultVertexAttributes(ShaderInterface *_shader);

	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);
	void pushVert(Vertex vertex);
};

class TriMesh : public MeshInterface{
public:
	void pushTri(GLubyte v0, GLubyte v1, GLubyte v2);
	TriMesh(GLenum polygonalDrawMode, GLenum drawMode):MeshInterface(polygonalDrawMode, drawMode){};
};

class QuadMesh : public MeshInterface{
public:
	void pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3);
	QuadMesh(GLenum polygonalDrawMode, GLenum drawMode):MeshInterface(polygonalDrawMode, drawMode){};
};