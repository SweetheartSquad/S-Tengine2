#pragma once
#include "vector3.h"
#include "Entity.h"
#include <iostream>
class Cube:public Entity
{
public:
	Cube(gmtl::Vec3f center, float size);
	~Cube(void);

	void draw(void);
	void update(void);

};

