#pragma once

#include <node\NodeShadable.h>

#include <shader\Shader.h>
#include <RenderOptions.h>

NodeShadable::NodeShadable(Shader * _shader) :
	shader(_shader)
{
	nodeType |= NodeType::kNODE_SHADABLE;
	ptrNodeShadable = this;

	if(shader != nullptr){
		shader->incrementReferenceCount();
	}
}

NodeShadable::~NodeShadable(){
	if(shader != nullptr){
		shader->decrementAndDelete();
		shader = nullptr;
	}
}

void NodeShadable::applyShader(RenderOptions * _renderOptions){
	if(_renderOptions->overrideShader == nullptr){
		_renderOptions->shader = shader;
	}else{
		_renderOptions->shader = _renderOptions->overrideShader;
	}
}