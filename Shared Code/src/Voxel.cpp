#pragma once

#include "Voxel.h"
#include "Joint.h"

void Voxel::render(MatrixStack * _matrixStack, RenderOptions * _renderStack){

}

Voxel::Voxel(ci::Vec3f _pos, Joint * _parent) :
	NodeRenderable(),
	NodeSelectable(),
	NodeHierarchical(_parent),
	pos(_pos)
{
	_parent->voxels.push_back(this);
}