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
#include "NodeRenderable.h"
#include "ShaderVariables.h"

class Texture;
class Material;
class MatrixStack;

class MeshInterface : public NodeRenderable{
public:
	/** Whether the vao, vbo, and ibo have been generated and initialized */
	bool loaded;
	/** Whether the vbo and ibo contain up-to-date vertex and index data */
	bool dirty;
	/** Vertex data for the vbo */
	std::vector<Vertex> vertices;
	/** Index data for the ibo */
	std::vector<GLuint> indices;
	/** Textures */
	std::vector<Texture *> textures;
	/** Materials */
	std::vector<Material *> materials;

public:
	/** ID of the vertex array object */
	GLuint vaoId;
	/** ID of the vertex buffer object */
	GLuint vboId;
	/** ID of the index buffer object */
	GLuint iboId;

	/** OpenGL memory hint; possible values:
	STATIC - The user will set the data once
	DYNAMIC - The user will set the data occasionally
	STREAM - The user will be changing the data after every use(or almost every use)
	*/
	GLenum drawMode;

	/** OpenGL render mode; possible values:
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

	/** Returns vertices.size() */
	GLsizei getVertCount();
	/** Returns sizeof(Vertex) */
	GLsizei getStride();

	bool shouldRenderLights;
	bool shouldRenderTextures;
	bool shouldRenderShadows;
	bool shouldRenderExtras;

	/**
	_polygonalDrawMode: STATIC, STREAM, DYNAMIC
	_drawMode: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
	*/
	MeshInterface(GLenum _polygonalDrawMode, GLenum _drawMode);
	~MeshInterface();

	/** If unloaded, generates the VAO, VBO, IBO and flags as loaded */
	void load();
	/** If loaded, deletes the VAO, VBO, IBO and flags as not loaded and dirty */
	void unload();
	/** If dirty, copies data from vertices and indices to VBO and IBO and flags as clean */
	void clean();
	/** Renders the vao using the given shader, model-view-projection and lights */
	virtual void render(MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
	/** Called render loop. Reders the textures for the mesh*/
	virtual void configureTextures(MatrixStack * _matrixStack, RenderOptions * _renderOption);
	/** Called render loop. Sets up the lights in the shader*/
	virtual void configureLights(MatrixStack * _matrixStack, RenderOptions * _renderOption);
	/** Sets up the model, view and projection matricies **/
	virtual void configureModelViewProjection(MatrixStack * _matrixStack, RenderOptions * _renderOption);
	virtual void configureShadows(MatrixStack * _matrixStack, RenderOptions * _renderOption);
	/**
	* Called render loop. Doesn't do anything in the base implementation of MeshInterface
	* This method can be overriden with any additional render logic. This prevents
	* the need for overriding the entire render loop
	*/
	virtual void configureExtras(MatrixStack * _matrixStack, RenderOptions * _renderOption);
	/** A helper method to configure all the starndard vertex attributes - Position, Colours, Normals */
	void configureDefaultVertexAttributes(Shader *_shader);
	/** Sets the normal of the given vert to _x, _y, _z */
	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);
	/** Sets the UV of the given vert to _x, _y */
	void setUV(unsigned long int _vertId, float _x, float _y);
	/** Adds _vertex to the list of vertices*/
	void pushVert(Vertex _vertex);
	void pushTexture2D(Texture * _texture);
	void pushMaterial(Material * _material);
};

/** MeshInterface preset for triangle meshes */
class TriMesh : public MeshInterface{
public:
	void pushTri(GLuint _v0, GLuint _v1, GLuint _v2);
	explicit TriMesh(GLenum _polygonalDrawMode, GLenum _drawMode = GL_TRIANGLES):MeshInterface(_polygonalDrawMode, _drawMode){};
};

/** MeshInterface preset for quad meshes */
class QuadMesh : public MeshInterface{
public:
	void pushQuad(GLuint _v0, GLuint _v1, GLuint _v2, GLuint _v3);
	explicit QuadMesh(GLenum _polygonalDrawMode, GLenum _drawMode = GL_QUADS):MeshInterface(_polygonalDrawMode, _drawMode){};
};