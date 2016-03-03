#pragma once

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentText.h>

// A pre-compiled default text shader. Has a text component and nothing else.
class ComponentShaderText : public ComponentShaderBase{
public:
	ShaderComponentText * textComponent;

	ComponentShaderText(bool _autoRelease);

	void setColor(float _r, float _g, float _b, float _a = 1.0f);
	glm::vec4 getColor();
};