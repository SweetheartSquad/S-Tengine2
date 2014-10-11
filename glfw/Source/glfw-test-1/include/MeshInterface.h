#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>

#include <vector>

#include "Vox.h"
#include "Shader.h"
#include "Vertex.h"
#include "Light.h"
#include "GLUtils.h"
#include "Texture.h"

class MeshInterface{
public:
	bool loaded;	// Whether the vao, vbo, and ibo have been generated and initialized
	bool dirty;		// Whether the vbo and ibo contain up-to-date vertex and index data
	std::vector<Vertex> vertices; // Vertex data for the vbo
	std::vector<GLubyte> indices; // Index data for the ibo
	std::vector<Texture *> textures; // Textures

public:
	GLuint vaoId;	// ID of the vertex array object
	GLuint vboId;	// ID of the vertex buffer object
	GLuint iboId;	// ID of the index buffer object

	/* OpenGL memory hint; possible values:
	STATIC - The user will set the data once
	DYNAMIC - The user will set the data occasionally
	STREAM - The user will be changing the data after every use(or almost every use)
	*/
	GLenum drawMode;

	/* OpenGL render mode; possible values:
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

	// Returns vertices.size()
	GLsizei getVertCount();
	// Returns sizeof(Vertex)
	GLsizei getStride();

	/*
	_polygonalDrawMode: STATIC, STREAM, DYNAMIC
	_drawMode: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
	*/
	MeshInterface(GLenum _polygonalDrawMode, GLenum _drawMode);
	~MeshInterface(void);

	// If unloaded, generates the VAO, VBO, IBO and flags as loaded
	void load();
	// If loaded, deletes the VAO, VBO, IBO and flags as not loaded and dirty
	void unload();
	// If dirty, copies data from vertices and indices to VBO and IBO and flags as clean
	void clean();
	// Renders the vao using the given shader, model-view-projection and lights
	void render(Shader *_shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*>);
	// Configures shader attributes (?)
	void configureVertexAttributes(GLint _vertexHandle, unsigned long int _arity, int _bufferOffset);
	// A helper method to configure all the starndard vertex attributes - Position, Colours, Normals
	void configureDefaultVertexAttributes(Shader *_shader);
	// Sets the normal of the given vert to _x, _y, _z
	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);
	// Sets the UV of the given vert to _x, _y
	void setUV(unsigned long int _vertId, float _x, float _y);
	// Adds _vertex to the list of vertices
	void pushVert(Vertex _vertex);
	void pushTexture2D(Texture* _texture);
};

// MeshInterface preset for triangle meshes
class TriMesh : public MeshInterface{
public:
	void pushTri(GLubyte _v0, GLubyte _v1, GLubyte _v2);
	TriMesh(GLenum _polygonalDrawMode, GLenum _drawMode = GL_TRIANGLES):MeshInterface(_polygonalDrawMode, _drawMode){};
};

// MeshInterface preset for quad meshes
class QuadMesh : public MeshInterface{
public:
	void pushQuad(GLubyte _v0, GLubyte _v1, GLubyte _v2, GLubyte _v3);
	QuadMesh(GLenum _polygonalDrawMode, GLenum _drawMode = GL_QUADS):MeshInterface(_polygonalDrawMode, _drawMode){};
};