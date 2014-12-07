#pragma once

#include "Voxel.h"
#include "Joint.h"

void Voxel::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){

}

Voxel::Voxel(ci::Vec3f _pos) :
	NodeRenderable(),
	NodeSelectable(),
	NodeChild(nullptr),
	pos(_pos)
{
}