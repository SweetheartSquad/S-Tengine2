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
#include "ShiftKiddie.h"

#include "Voxel.h"

class Step;

using namespace ci;

class Joint : public NodeAnimatable, public NodeHierarchical, public NodeRenderable, public NodeSelectable, public ShiftKiddie{
public:
	static unsigned long int nextId;

	std::vector<Voxel *> voxels;

	// Number of parent-child references from root to this (depth of 0 = root joint)
	unsigned long int depth;

	unsigned long int id;
	

	explicit Joint();
	explicit Joint(NodeParent * _parent);
	~Joint();
	


	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack);
	void update(Step * _step) override;
private:
	void init();
};
typedef std::pair<uint32_t, Joint*> JointPair;