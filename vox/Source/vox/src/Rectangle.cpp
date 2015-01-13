#pragma once

#include "Rectangle.h"
#include "Point.h"

vox::Rectangle::Rectangle(float _x, float _y, float _width, float _height){
	x      = _x;
	y      = _y;
	width  = _width;
	height = _height;
}

vox::Rectangle::~Rectangle(){
}

vox::Point vox::Rectangle::getTopLeft(){
	return Point(x, y);
}

vox::Point vox::Rectangle::getTopRight(){
	return vox::Point(x + width, y);
}

vox::Point vox::Rectangle::getBottomLeft(){
	return vox::Point(x, y + height);
}

vox::Point vox::Rectangle::getBottomRight(){
	return vox::Point(x + width, y + height);
}

float vox::Rectangle::getArea(){
	return width * height;
}

float vox::Rectangle::getPerimeter(){
	return 2 * (width + height);
}

bool vox::Rectangle::intersects(vox::Rectangle _rect){
	return (!getTopLeft().x < _rect.getTopRight().x && 
		    !getTopRight().x > _rect.getTopLeft().x &&
			!getTopLeft().y  < _rect.getBottomRight().y && 
			!getBottomRight().y > _rect.getTopLeft().y);
}