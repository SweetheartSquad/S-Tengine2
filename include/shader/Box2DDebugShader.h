#pragma once

#include "Shader.h"

class Box2DDebugShader : public Shader {
public:
	explicit Box2DDebugShader(bool _autoRelease);
	~Box2DDebugShader();

private:
	std::string getVertString();
	std::string getFragString();
};

