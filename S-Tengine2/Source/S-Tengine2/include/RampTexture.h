#pragma once
#include "ProgrammaticTexture.h"
#include <glm/glm.hpp>

class RampTexture : public ProgrammaticTexture {
public:
	glm::vec3 start, end;
	unsigned long int levels;
	RampTexture(glm::vec3 _start, glm::vec3 _end, unsigned long int _levels, bool autoRelease=true);

	void setRamp(glm::vec3 _start, glm::vec3 _end, unsigned long _levels);
};
