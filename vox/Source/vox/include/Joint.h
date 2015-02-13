#pragma once

#include <map>

#include <cinder\Vector.h>
#include <cinder\gl\gl.h>
#include <cinder\gl\GlslProg.h>

#include "node/NodeTransformable.h"
#include "node/NodeHierarchical.h"
#include "node/NodeAnimatable.h"
#include "node/NodeRenderable.h"
#include "node/NodeSelectable.h"
#include "node/ShiftKiddie.h"

#include "Voxel.h"

class Step;

using namespace ci;

class Joint : public NodeAnimatable, public NodeHierarchical, public NodeRenderable, public NodeSelectable, public ShiftKiddie{
public:
	unsigned long int id;
	static unsigned long int nextId;

	std::vector<Voxel *> voxels;

	

	explicit Joint();
	~Joint();


	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack);
	void update(Step * _step) override;
    // Since voxels can be parented to joints, but joints don't have voxels as children, we are overriding the add and remove child functions
    bool addChild(NodeChild * _child);
    // Since voxels can be parented to joints, but joints don't have voxels as children, we are overriding the add and remove child functions
    void addChildAtIndex(NodeChild * _child, int _index);
   // Since voxels can be parented to joints, but joints don't have voxels as children, we are overriding the add and remove child functions
     unsigned long int removeChild(NodeChild * _child);
};
typedef std::pair<uint32_t, Joint*> JointPair;