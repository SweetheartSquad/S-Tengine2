#pragma once

class Point;

class Rectangle{
public:

	float x, y, width, height;

	Rectangle(float _x, float _y, float _width, float _height);
	~Rectangle();

	Point getTopLeft();
	Point getTopRight();
	Point getBottomLeft();
	Point getBottomRight();

	float getArea();
	float getPerimeter();
	bool intersects(Rectangle _rect);
};