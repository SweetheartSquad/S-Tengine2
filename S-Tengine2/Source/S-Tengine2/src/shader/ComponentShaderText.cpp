#pragma once

#include <ComponentShaderText.h>

ComponentShaderText::ComponentShaderText() :
	textComponent(new ShaderComponentText(this))
{
	addComponent(textComponent);
	compileShader();
}