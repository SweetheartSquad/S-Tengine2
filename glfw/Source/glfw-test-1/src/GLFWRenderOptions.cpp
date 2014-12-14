#pragma once

#include "GLFWRenderOptions.h"

GLFWRenderOptions::GLFWRenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader):
	RenderOptions(_shader, _lights, _overrideShader),
	shadowMapTextureId(0),
	kc_active(false)
{
}

GLFWRenderOptions::~GLFWRenderOptions(){
}