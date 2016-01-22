#pragma once

#include <Rectangle.h>
#include <glm/vec3.hpp>

namespace sweet {
	// It's a bit strange to say that a 3d box is just a rectangle with z and depth, but it works so...
	class Box : public Rectangle{
	public:
		// returns the minimum box required to encompass _a and _b
		static sweet::Box bound(Box & _a, Box & _b);
		// returns a box that bounds vertices
		static sweet::Box bound(std::vector<glm::vec3> _verts);

		float z, depth;
		Box(float _x, float _y, float _z, float _width, float _height, float _depth);

		float getVolume();
		float getSurfaceArea();

		// sort of implemented
		bool intersects(Box _box);
		bool intersects(std::vector<glm::vec3> _verts);

		glm::vec3 getMinCoordinate();
		glm::vec3 getMaxCoordinate();
	};
};