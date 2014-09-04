#pragma once

#include "cinder\Vector.h"
#include "cinder\gl\gl.h"
#include "cinder/gl/GlslProg.h"
#include "Transform.h"
#include <map>

using namespace ci;

class Joint{
public:
	static uint32_t nextColor;
	static unsigned long int nextId;
	static std::map<uint32_t, Joint *> jointMap;
	Color color;

	unsigned long int depth;

	unsigned long int id;

	bool building;
	
	Joint * parent;
	std::vector<Joint *> children;
	Transform *transform;

	Joint();
	Joint(Joint * _parent);
	~Joint();
	
	void setPos(Vec3d _pos, bool _relative = true);
	Vec3d getPos(bool _relative = true);

	void draw(gl::GlslProg * _shader);
private:
	void init();
};
typedef std::pair<uint32_t, Joint*> JointPair;