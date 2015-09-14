#pragma once

#include <Box.h>
#include <algorithm>


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

vox::Box vox::Box::bound(vox::Box & _a, vox::Box & _b){
	vox::Box res(
		std::min(_a.x, _b.x),
		std::min(_a.y, _b.y),
		std::min(_a.z, _b.z),
		std::max(_a.x + _a.width, _b.x+_b.width),
		std::max(_a.y + _a.height, _b.y+_b.height),
		std::max(_a.z + _a.depth, _b.z+_b.depth)
	);
	res.width -= res.x;
	res.height -= res.y;
	res.depth -= res.z;
	return res;
}