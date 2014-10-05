#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "MeshInterface.h"
#include "Shader.h"
#include "Node.h"
#include "Vox.h"

class Entity : public Node{
public:
	MeshInterface * mesh;
	Transform * transform;
	Shader * shader;

	Entity * parent;
	std::vector<Entity *> children;

	explicit Entity(MeshInterface * _mesh = nullptr, Transform * _transform = nullptr, Shader * _shader = nullptr, Entity * _parent = nullptr);
	virtual ~Entity(void);

	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
	virtual void update();

	void addChild(Entity* _child);
	void removeChildAtIndex(int _index);
	void setParent(Entity *_parent);
	void setShader(Shader* _shader, bool _confiugreDefaultAttributes);
	// Calls unload on all children and on mesh
	void unload();
	void reset();
};
