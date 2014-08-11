#pragma once

#include "vector3.h"
#include "Entity.h"
#include "VertexBuffer.h"

#include <iostream>

class Cube:public Entity
{
public:
	Cube(glm::vec3 center, float size);
	~Cube(void);

	void draw(void) override;
	void update(void) override;

};

