#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "MeshInterface.h"
#include "ShaderInterface.h"
#include "Node.h"
#include "Vox.h"

class Entity : public Node{
public:
	MeshInterface * mesh;
	Transform * transform;
	ShaderInterface * shader;

	Entity * parent;
	std::vector<Entity *> children;

	explicit Entity(MeshInterface * _mesh = nullptr, Transform * _transform = nullptr, ShaderInterface * _shader = nullptr, Entity * _parent = nullptr);
	virtual ~Entity(void);

	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
	virtual void update();

	void addChild(Entity* _child);
	void removeChildAtIndex(int _index);
	void setParent(Entity *_parent);
	void setShader(ShaderInterface* _shader, bool _confiugreDefaultAttributes);
	//calls unload on all children and on mesh
	void unload();
	void reset();
};
