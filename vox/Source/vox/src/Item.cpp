#pragma once

#include "Item.h"

Item ::Item (Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int _damage, float _handleX, float _handleY):
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	Box2DSuperSprite(_world, _categoryBits, _maskBits),
	damage(_damage),
	handleX(_handleX),
	handleY(_handleY)
{
}

Item ::~Item (){
}