#pragma once

#include "Rectangle.h"
#include "Point.h"

Rectangle::Rectangle(float _x, float _y, float _width, float _height){
	x      = _x;
	y      = _y;
	width  = _width;
	height = _height;
}

Rectangle::~Rectangle(){
}

Point Rectangle::getTopLeft(){
	return Point(x, y);
}

Point Rectangle::getTopRight(){
	return Point(x + width, y);
}

Point Rectangle::getBottomLeft(){
	return Point(x, y + height);
}

Point Rectangle::getBottomRight(){
	return Point(x + width, y + height);
}

float Rectangle::getArea(){
	return width * height;
}

float Rectangle::getPerimeter(){
	return 2 * (width + height);
}

bool Rectangle::intersects(Rectangle _rect){
	return (!getTopLeft().x < _rect.getTopRight().x && 
		    !getTopRight().x > _rect.getTopLeft().x &&
			!getTopLeft().y  < _rect.getBottomRight().y && 
			!getBottomRight().y > _rect.getTopLeft().y);
}