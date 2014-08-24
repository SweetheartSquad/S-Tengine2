#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ShaderInterface.h"

class Entity{

public:
	VertexBuffer *vertexBuffer;
	std::vector<Entity*> *children;
	Entity *parent;
	ShaderInterface *shader;
	std::vector<Vertex>*vertices;

	Transform *transform;

	Entity(void);
	virtual ~Entity(void);

	virtual void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	virtual void update();
	
	void pushVert(Vertex vertex);
	void addChild(Entity* child);
	void removeChildAtIndex(int index);
	void setParent(Entity *parent);
};

