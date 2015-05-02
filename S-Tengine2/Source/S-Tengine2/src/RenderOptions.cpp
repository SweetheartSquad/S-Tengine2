#pragma once 

#include "RenderOptions.h"

RenderOptions::RenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader) :
	lights(_lights),
	shader(_shader),
	overrideShader(_overrideShader),
	alphaSorting(false),
	currentVao(0),
	currentlyBoundShaderId(0)
{
}

RenderOptions::~RenderOptions(){
}