#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>

#include <vector>

#include "Vox.h"
#include "shader/Shader.h"
#include "Vertex.h"
#include "Light.h"
#include "GLUtils.h"
#include <node\NodeRenderable.h>
#include <node\NodeLoadable.h>
#include <node\NodeResource.h>
#include "shader/ShaderVariables.h"
#include <Box.h>

class Texture;
class Material;
class MatrixStack;


class MeshInterface : public virtual NodeRenderable, public virtual NodeLoadable, public virtual NodeResource{
public:
	/** Whether the vbo and ibo contain up-to-date vertex and index data */
	bool dirty;
	bool texturesDirty;
	/** Vertex data for the vbo */
	std::vector<Vertex> vertices;
	/** Index data for the ibo */
	std::vector<GLuint> indices;
	/** Materials */
	std::vector<Material *> materials;
	/**
	GL_REPEAT
	GL_MIRRORED_REPEAT
	GL_CLAMP_TO_EDGE
	GL_CLAMP_TO_BORDER
	*/
	GLenum uvEdgeMode;
	/**
	The texture magnification function is used when the pixel being textured maps to an area less than or equal to one texture element.
	i.e. Filter when scaling up textures

	GL_NEAREST: Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	GL_LINEAR: Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	*/
	GLenum scaleModeMag;

	/**
	The texture minifying function is used whenever the pixel being textured maps to an area greater than one texture element.
	i.e. Filter when scaling down textures

	GL_NEAREST: Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	GL_LINEAR: Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	GL_NEAREST_MIPMAP_NEAREST: Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_NEAREST criterion (the texture element nearest to the center of the pixel) to produce a texture value.
	GL_LINEAR_MIPMAP_NEAREST: Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_LINEAR criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value.
	GL_NEAREST_MIPMAP_LINEAR: Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_NEAREST criterion (the texture element nearest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
	GL_LINEAR_MIPMAP_LINEAR: Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_LINEAR criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
	*/
	GLenum scaleModeMin;

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
	/**
	_polygonalDrawMode: STATIC, STREAM, DYNAMIC
	_drawMode: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
	*/
	MeshInterface(GLenum _polygonalDrawMode, GLenum _drawMode);
	virtual ~MeshInterface();

	/** If unloaded, generates the VAO, VBO, IBO and flags as loaded */
	void load() override;
	/** If loaded, deletes the VAO, VBO, IBO and flags as not loaded and dirty */
	void unload() override;
	/** If dirty, copies data from vertices and indices to VBO and IBO and flags as clean */
	void clean();
	/** Renders the vao using the given shader, model-view-projection and lights */
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
	/** A helper method to configure all the starndard vertex attributes - Position, Colours, Normals */
	void configureDefaultVertexAttributes(Shader *_shader);
	/** Sets the normal of the given vert to _x, _y, _z */
	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);
	/** Sets the UV of the given vert to _x, _y */
	void setUV(unsigned long int _vertId, float _x, float _y);
	/** Adds _vertex to the list of vertices*/
	void pushVert(Vertex _vertex);
	void pushTexture2D(Texture * _texture);
	void popTexture2D();
	void removeTextureAt(int _idx);
	void pushMaterial(Material * _material);
	unsigned long int textureCount();
	Texture * getTexture(int _idx);

	// returns a box which covers the verts of the mesh
	vox::Box calcBoundingBox();

private:
	/** Textures */
	std::vector<Texture *> textures;
};

/** MeshInterface preset for triangle meshes */
class TriMesh : public MeshInterface{
public:
	void pushTri(GLuint _v0, GLuint _v1, GLuint _v2);
	explicit TriMesh(GLenum _polygonalDrawMode = GL_TRIANGLES, GLenum _drawMode = GL_STATIC_DRAW) : MeshInterface(_polygonalDrawMode, _drawMode), NodeResource(true){};
};

/** MeshInterface preset for quad meshes */
class QuadMesh : public MeshInterface{
public:
	void pushQuad(GLuint _v0, GLuint _v1, GLuint _v2, GLuint _v3);
	explicit QuadMesh(GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW) : MeshInterface(_polygonalDrawMode, _drawMode), NodeResource(true){};
};