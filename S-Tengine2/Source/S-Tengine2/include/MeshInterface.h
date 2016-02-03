#pragma once

#include <glm\glm.hpp>

#include <vector>
#include <ostream>

#include "Sweet.h"
#include "shader/Shader.h"
#include "Vertex.h"
#include "Light.h"
#include "GLUtils.h"
#include <node\NodeRenderable.h>
#include <node\NodeLoadable.h>
#include <node\NodeResource.h>
#include "shader/ShaderVariables.h"
#include <Box.h>

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

class Texture;
class Material;
class MatrixStack;

class MeshInterface : public virtual NodeRenderable, public virtual NodeLoadable, public virtual NodeResource, public virtual NodeChild{
public:
	/** Whether the vbo and ibo contain up-to-date vertex and index data */
	bool dirty;
	/** Textures */
	std::vector<Texture *> textures;
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
	_polygonalDrawMode: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
	_drawMode: STATIC, STREAM, DYNAMIC
	*/
	MeshInterface(GLenum _polygonalDrawMode, GLenum _drawMode);
	virtual ~MeshInterface();

	/** If unloaded, generates the VAO, VBO, IBO and flags as loaded */
	virtual void load() override;
	/** If loaded, deletes the VAO, VBO, IBO and flags as not loaded and dirty */
	virtual void unload() override;
	/** If dirty, copies data from vertices and indices to VBO and IBO and flags as clean */
	virtual void clean();
	/** Renders the vao using the given shader, model-view-projection and lights */
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
	/** A helper method to configure all the starndard vertex attributes - Position, Colours, Normals */
	void configureDefaultVertexAttributes(Shader *_shader);
	/** Sets the normal of the given vert to _x, _y, _z */
	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);

	// returns the normal vector to the plane formed by _v1, _v2, and _v3
	glm::vec3 calcNormal(unsigned long int _v1, unsigned long int _v2, unsigned long int _v3) const;
	// calculates the normal vector to the plane formed by _v1, _v2, and _v3, and sets each vertex to use it as their normal
	// also returns the normal vector
	glm::vec3 setAutoNormal(unsigned long int _v1, unsigned long int _v2, unsigned long int _v3);

	/** Sets the UV of the given vert to _x, _y */
	void setUV(unsigned long int _vertId, float _x, float _y);
	/** Adds _vertex to the list of vertices*/
	void pushVert(Vertex _vertex);
	void pushTexture2D(Texture * _texture);

	// sets both scaleModeMin and scaleModeMag to _both
	// Note that only GL_NEAREST and GL_LINEAR are valid options
	void setScaleMode(GLenum _both);
	
	// removes the last texture pushed onto the mesh
	// decrements and attempts to delete the texture
	// if the texture is deleted, returns nullptr
	// otherwise, returns the popped texture
	Texture * popTexture2D();

	// calls popTexture2D until there aren't any textures left
	void clearTextures();
	// calls clearTextures() and then pushTexture2D(_newTexture)
	void replaceTextures(Texture * _newTexture);

	void removeTextureAt(int _idx);
	void pushMaterial(Material * _material);
	unsigned long int textureCount() const;
	Texture * getTexture(int _idx) const;
	
	// returns a box which covers the verts of the mesh
	sweet::Box calcBoundingBox() const;

	// multiplies the mesh's vertices by the transformation matrix of the provided _transform
	void applyTransformation(Transform * _transform);

	// appends all of _mesh's vertices and indices (indices are offset by current vertex count) to this mesh
	// NOTE: does not delete _mesh
	// ALSO NOTE: transferring verts from a TriMesh to a QuadMesh or vice versa probably won't do what you want
	void insertVertices(const MeshInterface * const _mesh);

	friend std::ostream& operator<<(std::ostream& os, const MeshInterface& obj);
};

class QuadMesh;

/** MeshInterface preset for triangle meshes */
class TriMesh : public MeshInterface{
public:
	// creates a triangulated version of _mesh
	explicit TriMesh(const QuadMesh * const _mesh, bool _autoRelease);
	void pushTri(GLuint _v0, GLuint _v1, GLuint _v2);
	explicit TriMesh(bool _autorelease, GLenum _polygonalDrawMode = GL_TRIANGLES, GLenum _drawMode = GL_STATIC_DRAW) : MeshInterface(_polygonalDrawMode, _drawMode), NodeResource(_autorelease){};
};

/** MeshInterface preset for quad meshes */
class QuadMesh : public MeshInterface{
public:
	void pushQuad(GLuint _v0, GLuint _v1, GLuint _v2, GLuint _v3);
	explicit QuadMesh(bool _autorelease, GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW) : MeshInterface(_polygonalDrawMode, _drawMode), NodeResource(_autorelease){};
};