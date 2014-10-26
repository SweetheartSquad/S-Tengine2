#pragma once

#include <map>

#include <cinder\Vector.h>
#include <cinder\gl\gl.h>
#include <cinder\gl\GlslProg.h>

#include "NodeTransformable.h"
#include "NodeHierarchical.h"

using namespace ci;

class Joint : public NodeTransformable, public NodeHierarchical{
public:
	static uint32_t nextColor;
	static unsigned long int nextId;
	static std::map<uint32_t, Joint *> jointMap;
	Color color;

	// Number of parent-child references from root to this
	unsigned long int depth;

	unsigned long int id;
	
	// Recursively delete all the children of a joint, then delete the joint itself
	static void deleteJoints(NodeHierarchical * _j);

	explicit Joint();
	explicit Joint(NodeHierarchical * _parent);
	~Joint();
	
	void setPos(Vec3d _pos, bool _convertToRelative = true);
	Vec3d getPos(bool _relative = true);

	void draw(gl::GlslProg * _shader);
private:
	void init();
};
typedef std::pair<uint32_t, Joint*> JointPair;