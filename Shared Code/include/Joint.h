#pragma once

#include "cinder\Vector.h"
#include "cinder\gl\gl.h"
#include "cinder/gl/GlslProg.h"
#include <map>

using namespace ci;

class Joint{
public:
	static uint32_t nextColor;
	static unsigned long int nextId;
	static std::map<uint32_t, Joint *> jointMap;

	Quatd orientation;
	Color color;

	unsigned long int depth;

	unsigned long int id;

	bool building;
	
	Joint * parent;
	std::vector<Joint *> children;
	
	Joint();
	Joint(Joint * _parent);
	~Joint();
	
	void setPos(Vec3d _pos, bool _relative = true);
	Vec3d getPos(bool _relative = true);

	void draw(gl::GlslProg * _shader);
private:
	void init();
	Vec3d pos;
};
typedef std::pair<uint32_t, Joint*> JointPair;