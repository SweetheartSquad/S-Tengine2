#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "MeshInterface.h"
#include "Shader.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "Vox.h"
#include "NodeAnimatable.h"

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class Entity : public NodeAnimatable, public NodeHierarchical{
public:

	/** Reference to this entity's mesh */
	MeshInterface * mesh;
	/** Reference to this entity's shader */
	Shader * shader;

	explicit Entity(MeshInterface * _mesh = nullptr, Transform * _transform = nullptr, Shader * _shader = nullptr, Entity * _parent = nullptr);
	virtual ~Entity(void);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void draw(MatrixStack * _matrixStack, RenderOptions * _renderStack);
	/** Doesn't do anything by default */
	virtual void update();

	void addChild(Entity * _child);
	void removeChildAtIndex(int _index);
	void setParent(Entity *_parent);
	/**Sets shader to _shader*/
	void setShader(Shader* _shader, bool _confiugreDefaultAttributes);
	/** Recursivley sets the shader to _shader for _entity's children recursivley*/
	void setShaderOnChildren(Shader * _shader);
	/** Calls unload on all children and on mesh */
	void unload();
	/** Loads and cleans mesh, configures default vertex attributes, deletes and reloads shader, and calls unload on all children (recursive) */
	void reset();
};
