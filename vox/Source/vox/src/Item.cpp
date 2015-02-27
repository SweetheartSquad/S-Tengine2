#pragma once

#include "Item.h"

Item::Item(bool _pickupable, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, int _damage, float _handleX, float _handleY):
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	damage(_damage),
	handleX(_handleX),
	handleY(_handleY),
	pickupable(_pickupable)
{
}

Item ::~Item (){
}