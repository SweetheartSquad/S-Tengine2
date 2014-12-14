#pragma once

#include "VoxRenderOptions.h"

VoxRenderOptions::VoxRenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader):
	RenderOptions(_shader, _lights, _overrideShader),
	shadowMapTextureId(0),
	kc_active(false)
{
}

VoxRenderOptions::~VoxRenderOptions(){
}