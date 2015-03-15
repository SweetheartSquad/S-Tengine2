#pragma once

#include <shader/GeometryComponent.h>
#include <shader/Shader.h>

GeometryComponent::GeometryComponent(Shader * _shader) :
	shader(_shader),
	dirty(true)
{
}

GeometryComponent::~GeometryComponent(){
}

void GeometryComponent::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	if(dirty){
		configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
		dirty = false;
	}
}
void GeometryComponent::makeDirty(){
	if(shader != nullptr){
		shader->makeDirty();
	}
	dirty = true;
}