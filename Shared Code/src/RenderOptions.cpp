#include "RenderOptions.h"

RenderOptions::RenderOptions(){

}

RenderOptions::RenderOptions(Shader* _shader, std::vector<Light *> *_lights):
	shader(_shader),
	lights(_lights),
	shadowMapTextureId(0)
{
}

RenderOptions::~RenderOptions(){
}