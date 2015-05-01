#pragma once

#include <Box.h>

vox::Box::Box(float _x, float _y, float _z, float _width, float _height, float _depth) :
	Rectangle(_x, _y, _width, _height)
{
	x		= _x;
	y		= _y;
	z		= _z;
	width	= _width;
	height	= _height;
	depth	= _depth;
}


float vox::Box::getSurfaceArea(){
	return (width * height + width * depth + height * depth) * 2.f;
}

float vox::Box::getVolume(){
	return width * height * depth;
}

bool vox::Box::intersects(vox::Box _rect){
	return false;
}