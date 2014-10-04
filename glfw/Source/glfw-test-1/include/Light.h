#pragma once

#include <glm/glm.hpp>

#include <Node.h>

struct LightData {
	glm::vec3 position;
	glm::vec3 intensities;
};

class Light : public Node{
public:
	Light();
	~Light();

	LightData data;
};