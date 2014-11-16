#pragma once

#include <cinder/Vector.h>

#include "NodeRenderable.h"
#include "NodeSelectable.h"

class Joint;

class Voxel : public NodeRenderable, public NodeSelectable{
public:
	Joint * parent;
	ci::Vec3f pos;
	virtual void render(MatrixStack * _matrixStack, RenderOptions * _renderStack);

	Voxel(ci::Vec3f _pos, Joint * _parent);
};