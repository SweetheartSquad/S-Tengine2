#pragma once

#include <shader/ShaderComponent.h>
#include <shader/ComponentShaderBase.h>

ShaderComponent::ShaderComponent(ComponentShaderBase * _shader) :
	shader(_shader),
	dirty(true)
{
}

ShaderComponent::~ShaderComponent(){
}

void ShaderComponent::clean(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable){
	if(dirty){
		configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
		dirty = false;
	}
}

void ShaderComponent::makeDirty(){
	if(shader != nullptr){
		shader->makeDirty();
	}
	dirty = true;
}