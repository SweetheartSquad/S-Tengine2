#pragma once

#include "Shader.h"

class BlurShader : public Shader{
public:
	explicit BlurShader(bool _autoRelease);
private:
	std::string getVertString();
	std::string getFragString();
};

