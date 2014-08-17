#pragma once

#include "cinder\Vector.h"
#include "cinder\gl\gl.h"
#include <map>

using namespace ci;

class Joint{
public:
	static uint32_t nextColor;
	static unsigned long int nextId;
	static std::map<uint32_t, Joint *> jointMap;

	Vec3d pos;
	Quatd rotation;
	Color color;

	unsigned long int depth;

	unsigned long int id;

	bool building;
	
	Joint * parent;
	std::vector<Joint *> children;
	
	void init();
	Joint();
	Joint(Joint * _parent);
	~Joint();

	void draw();
};
typedef std::pair<uint32_t, Joint*> JointPair;