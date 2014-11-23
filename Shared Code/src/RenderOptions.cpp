#include "RenderOptions.h"

RenderOptions::RenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader):
	shader(_shader),
	lights(_lights),
	overrideShader(_overrideShader)/*,
	shadowMapTextureId(0)*/
{
}

RenderOptions::~RenderOptions(){
}