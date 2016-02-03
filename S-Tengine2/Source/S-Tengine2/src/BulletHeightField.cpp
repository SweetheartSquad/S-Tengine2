#pragma once

#include <BulletHeightField.h>
#include <Texture.h>
#include <TextureUtils.h>

BulletHeightFieldShape::BulletHeightFieldShape(Texture * _heightMap, glm::vec3 _scale, unsigned long int _upAxis) :
	heightMap(_heightMap),
	btHeightfieldTerrainShape(_heightMap->width, _heightMap->height, _heightMap->data, (_upAxis == 0 ? _scale.x : (_upAxis == 1 ? _scale.y : _scale.z)), sweet::TextureUtils::getMin(_heightMap), sweet::TextureUtils::getMax(_heightMap), _upAxis, PHY_ScalarType::PHY_UCHAR, false)
{
	++_heightMap->referenceCount;
	setLocalScaling(btVector3(_upAxis == 0 ? 1 : _scale.x, _upAxis == 1 ? 1 : _scale.y, _upAxis == 2 ? 1 : _scale.z));
}

BulletHeightFieldShape::~BulletHeightFieldShape(){
	heightMap->decrementAndDelete();
}