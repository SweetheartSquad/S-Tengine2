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

glm::vec4 ComponentShaderText::getColor(){
	return textComponent->getColor();
}

void ComponentShaderText::setColor(float _r, float _g, float _b, float _a) {
	textComponent->setColor(glm::vec4(_r, _g, _b, _a));
	makeDirty();
}
