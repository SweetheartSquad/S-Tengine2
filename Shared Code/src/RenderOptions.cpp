#include "RenderOptions.h"

RenderOptions::RenderOptions(){

}

RenderOptions::RenderOptions(Shader* _shader, std::vector<Light *> *_lights):
	shader(_shader),
	lights(_lights)
{
}

RenderOptions::~RenderOptions(){
}