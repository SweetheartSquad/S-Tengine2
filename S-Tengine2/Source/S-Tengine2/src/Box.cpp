#pragma once

#include <Box.h>
#include <algorithm>


#include <algorithm>

sweet::Box::Box(float _x, float _y, float _z, float _width, float _height, float _depth) :
	Rectangle(_x, _y, _width, _height)
{
	x		= _x;
	y		= _y;
	z		= _z;
	width	= _width;
	height	= _height;
	depth	= _depth;
}


float sweet::Box::getSurfaceArea(){
	return (width * height + width * depth + height * depth) * 2.f;
}

float sweet::Box::getVolume(){
	return width * height * depth;
}

bool sweet::Box::intersects(sweet::Box _rect){
	if (_rect.x + _rect.width < x) return false; // rect is left
    if (_rect.x > x + width) return false; // rect is right
    if (_rect.y + _rect.height < y) return false; // rect is above
    if (_rect.y > y + height) return false; // rect is below
	if (_rect.z + _rect.depth < z) return false; // rect is in front
    if (_rect.z > z + depth) return false; // rect is behind

    return true; // boxes overlap
}

bool sweet::Box::intersects(std::vector<glm::vec3> _verts){
	return intersects(sweet::Box::bound(_verts));
}

sweet::Box sweet::Box::bound(sweet::Box & _a, sweet::Box & _b){
	sweet::Box res(
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

sweet::Box sweet::Box::bound(std::vector<glm::vec3> _verts){
	float minX = 99999, minY= 99999, minZ = 99999,
		maxX = -99999, maxY = -99999, maxZ = -99999;

	for(auto i : _verts){
		maxX = std::max(i.x, maxX);
		maxY = std::max(i.y, maxY);
		maxZ = std::max(i.z, maxZ);

		minX = std::min(i.x, minX);
		minY = std::min(i.y, minY);
		minZ = std::min(i.z, minZ);
	}

	return sweet::Box(minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ);
}

glm::vec3 sweet::Box::getMinCoordinate(){
	return glm::vec3(x, y, z);
}

glm::vec3 sweet::Box::getMaxCoordinate(){
	return glm::vec3(x + width, y + height, z + depth);
}

std::vector<glm::vec3> sweet::Box::getVertices(){
	glm::vec3 min = getMinCoordinate();
	glm::vec3 max = getMaxCoordinate();

	std::vector<glm::vec3> vertices;
	vertices.push_back(min);							// left bottom back
	vertices.push_back(glm::vec3(max.x, min.y, min.z));	// right bottom back
	vertices.push_back(glm::vec3(min.x, max.y, min.z));	// left top back
	vertices.push_back(glm::vec3(max.x, max.y, min.z));	// right top back
	vertices.push_back(glm::vec3(min.x, min.y, max.z));	// left bottom front
	vertices.push_back(glm::vec3(max.x, min.y, max.z));	// right bottom front
	vertices.push_back(glm::vec3(min.x, max.y, max.z));	// left top front
	vertices.push_back(max);							// right top front

	return vertices;
}