#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "MeshInterface.h"
#include "Shader.h"
#include "Node.h"
#include "Vox.h"

// A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children
class Entity : public Node{
public:

	MeshInterface * mesh;			// Reference to this entity's mesh
	Transform * transform;			// Reference to this entity's transform
	Shader * shader;				// Reference to this entity's shader
	Entity * parent;				// Reference to this entity's parent
	std::vector<Entity *> children;	// List of references to child entities

	explicit Entity(MeshInterface * _mesh = nullptr, Transform * _transform = nullptr, Shader * _shader = nullptr, Entity * _parent = nullptr);
	virtual ~Entity(void);

	/*
	Pushes model matrix stack,
	Applies the model matrix of transform,
	Loads and cleans mesh (if necessary),
	Renders mesh,
	Calls draw on children (recursive),
	Pops model matrix stack
	*/
	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
	// Doesn't do anything by default
	virtual void update();

	void addChild(Entity * _child);
	void removeChildAtIndex(int _index);
	void setParent(Entity *_parent);
	void setShader(Shader* _shader, bool _confiugreDefaultAttributes);
	// Calls unload on all children and on mesh
	void unload();
	// Loads and cleans mesh, configures default vertex attributes, deletes and reloads shader, and calls unload on all children (recursive)
	void reset();
};
