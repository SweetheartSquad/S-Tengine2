#pragma once

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentText.h>

// A pre-compiled default text shader. Has a text component and nothing else.
class ComponentShaderText : public ComponentShaderBase{
public:
	ShaderComponentText * textComponent;

	ComponentShaderText(bool _autoRelease);
};