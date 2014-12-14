#pragma once

#include <cinder/Vector.h>

#include "NodeRenderable.h"
#include "NodeSelectable.h"
#include "NodeHierarchical.h"
#include "ShiftKiddie.h"

class Joint;

class Voxel : public NodeRenderable, public NodeSelectable, public ShiftKiddie{
public:
	//ci::Vec3f pos;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack);
	
	Voxel(ci::Vec3f _pos, Joint * _parent, bool _convertToRelative);
	~Voxel();
};