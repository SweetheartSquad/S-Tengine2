#include "CinderRenderOptions.h"

CinderRenderOptions::CinderRenderOptions(Shader* _shader, std::vector<Light *> *_lights, Shader * _overrideShader):
	RenderOptions(_shader, _lights, _overrideShader)
{

}

CinderRenderOptions::~CinderRenderOptions(){
}