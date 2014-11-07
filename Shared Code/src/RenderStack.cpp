#include "RenderStack.h"

RenderStack::RenderStack(){

}

RenderStack::RenderStack(Shader* _shader, std::vector<Light *> *_lights):
	shader(_shader),
	lights(_lights)
{
}

RenderStack::~RenderStack(){
}