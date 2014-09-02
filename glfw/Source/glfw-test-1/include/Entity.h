#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "VertexInterface.h"
#include "ShaderInterface.h"
#include "Node.h"
#include "Vox.h"

class Entity:public Node{
public:

	VertexInterface * vertexInterface;
	std::vector<Entity*> *children;
	Entity *parent;
	ShaderInterface *shader;

	Transform *transform;

	Entity(void);
	virtual ~Entity(void);

	virtual void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	virtual void update();
	
	void pushVert(Vertex vertex);
	void pushTri(GLubyte v0, GLubyte v1, GLubyte v2);
	void pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3);
	void setNormal(unsigned long int _vertId, float _x, float _y, float _z);

	void addChild(Entity* child);
	void removeChildAtIndex(int index);
	void setParent(Entity *parent);
};

