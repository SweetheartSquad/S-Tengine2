#pragma once

#include "cinder\Vector.h"
#include "cinder\gl\gl.h"

using namespace ci;

class Bone{
public:
	Vec3d start, end;

	double rotation;

	bool building;
	
	Bone * parent;
	std::vector<Bone *> children;
	
	Bone();
	~Bone();

	void draw();
};