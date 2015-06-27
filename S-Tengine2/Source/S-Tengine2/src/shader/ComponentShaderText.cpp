#pragma once

#include <shader\ComponentShaderText.h>

ComponentShaderText::ComponentShaderText(bool _autoRelease) :
	ComponentShaderBase(_autoRelease),
	textComponent(new ShaderComponentText(this))
{
	addComponent(textComponent);
	compileShader();
	load();
}

void ComponentShaderText::setColor(float _r, float _g, float _b) {
	textComponent->setColor(glm::vec3(_r, _g, _b));
	makeDirty();
}