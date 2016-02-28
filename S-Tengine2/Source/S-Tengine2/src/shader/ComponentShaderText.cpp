#pragma once

#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentMVP.h>
#include <shader/ShaderComponentDepthOffset.h>

ComponentShaderText::ComponentShaderText(bool _autoRelease) :
	ComponentShaderBase(_autoRelease),
	textComponent(new ShaderComponentText(this))
{
	addComponent(textComponent);
	addComponent(new ShaderComponentMVP(this));
	addComponent(new ShaderComponentDepthOffset(this));
	compileShader();
	load();
}

void ComponentShaderText::setColor(float _r, float _g, float _b) {
	textComponent->setColor(glm::vec3(_r, _g, _b));
	makeDirty();
}