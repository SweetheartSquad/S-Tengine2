#pragma once

#include <map>

#include <cinder\Vector.h>
#include <cinder\gl\gl.h>
#include <cinder\gl\GlslProg.h>

#include "NodeTransformable.h"
#include "NodeHierarchical.h"
#include "NodeAnimatable.h"
#include "NodeRenderable.h"
#include "NodeSelectable.h"

#include "Voxel.h"

class Step;

using namespace ci;

class Joint : public NodeAnimatable, public NodeHierarchical, public NodeRenderable, public NodeSelectable{
public:
	static unsigned long int nextId;

	std::vector<Voxel *> voxels;

	// Number of parent-child references from root to this (depth of 0 = root joint)
	unsigned long int depth;

	unsigned long int id;
	

	explicit Joint();
	explicit Joint(NodeParent * _parent);
	~Joint();
	

	// Sets the translation vector of the joint
	void setPos(Vec3d _pos, bool _convertToRelative = true);

	// Returns the translation vector of the joint (if _relative is false, applies all of the transformations of the parent joints before returning the vector)
	Vec3d getPos(bool _relative = true);

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack);
	void update(Step * _step) override;
private:
	void init();
};
typedef std::pair<uint32_t, Joint*> JointPair;