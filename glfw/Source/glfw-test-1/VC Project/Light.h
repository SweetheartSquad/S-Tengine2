#pragma once

#include <glm/glm.hpp>

#include "Transform.h"

class Light : public Node{
public:
	Light();
	~Light();

	glm::vec3 position;
	glm::vec3 intensities;

};