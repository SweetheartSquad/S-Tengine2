#pragma once
#include "ProgrammaticTexture.h"
#include <glm/glm.hpp>

class RampTexture : public ProgrammaticTexture {
public:
	RampTexture(glm::vec3 _start, glm::vec3 _end, unsigned long int _levels, bool autoRelease=true);
};
