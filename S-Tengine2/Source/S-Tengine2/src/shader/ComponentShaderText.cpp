#pragma once

#include <shader\ComponentShaderText.h>

ComponentShaderText::ComponentShaderText(bool _autoRelease) :
	ComponentShaderBase(_autoRelease),
	textComponent(new ShaderComponentText(this))
{
	addComponent(textComponent);
	compileShader();
}