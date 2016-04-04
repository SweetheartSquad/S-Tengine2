#pragma once

#include "Rectangle.h"
#include "Point.h"

sweet::Rectangle::Rectangle(float _x, float _y, float _width, float _height){
	x      = _x;
	y      = _y;
	width  = _width;
	height = _height;
}

sweet::Rectangle::~Rectangle(){
}

sweet::Point sweet::Rectangle::getTopLeft(){
	return Point(x, y+height);
}

sweet::Point sweet::Rectangle::getTopRight(){
	return sweet::Point(x + width, y+height);
}

sweet::Point sweet::Rectangle::getBottomLeft(){
	return sweet::Point(x, y);
}

sweet::Point sweet::Rectangle::getBottomRight(){
	return sweet::Point(x + width, y);
}

float sweet::Rectangle::getArea(){
	return width * height;
}

float sweet::Rectangle::getPerimeter(){
	return 2 * (width + height);
}

bool sweet::Rectangle::intersects(sweet::Rectangle _rect){
	bool aLeftOfB  = getTopRight().x < _rect.getTopLeft().x;
	bool aRightOfB = getTopLeft().x > _rect.getTopRight().x;
	bool aAboveB = getBottomLeft().y > _rect.getTopLeft().y;
	bool aBelowB = getTopLeft().y < _rect.getBottomLeft().y;

	return !( aLeftOfB || aRightOfB || aAboveB || aBelowB );
}