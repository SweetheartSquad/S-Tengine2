#pragma once 

#include "RenderOptions.h"

RenderOptions::RenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader) :
	lights(_lights),
	shader(_shader),
	overrideShader(_overrideShader),
	alphaSorting(false),
	currentVao(0),
	currentlyBoundShaderId(0),
	depthBufferId(0),
	normalBufferId(0),
	alphaEnabled(true),
	depthEnabled(true)
{
	setClearColour(0,0,0,1);
}

RenderOptions::~RenderOptions(){
}

void RenderOptions::clear(){
	glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderOptions::setClearColour(float _r, float _g, float _b, float _a){
	clearColour[0] = _r;
	clearColour[1] = _g;
	clearColour[2] = _b;
	clearColour[3] = _a;
}