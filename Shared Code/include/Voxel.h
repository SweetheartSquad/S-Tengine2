#pragma once

#include "NodeRenderable.h"
#include <cinder/Vector.h>

class Voxel : public NodeRenderable{
public:
	ci::Vec3f pos;
	virtual void render(MatrixStack * _matrixStack, RenderOptions * _renderStack);
};