#pragma once

#include <Rectangle.h>

namespace vox {
	// It's a bit strange to say that a 3d box is just a rectangle with z and depth, but it works so...
	class Box : public Rectangle{
	public:
		float z, depth;
		Box(float _x, float _y, float _z, float _width, float _height, float _depth);

		float getVolume();
		float getSurfaceArea();

		// unimplemented
		bool intersects(Box _box);
	};
};