#pragma once

#include "Point.h"
#include <node\Node.h>

namespace sweet {
	class Rectangle : public Node{
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
};