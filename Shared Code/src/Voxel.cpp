#pragma once

#include "Voxel.h"
#include "Joint.h"
#include "Transform.h"

void Voxel::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){

}

Voxel::Voxel(ci::Vec3f _pos, Joint * _parent, bool _convertToRelative) :
	NodeRenderable(),
	NodeSelectable(),
	NodeTransformable(new Transform()),
	NodeChild(_parent)
{
    setPos(glm::vec3(_pos.x, _pos.y, _pos.z), _convertToRelative);
}