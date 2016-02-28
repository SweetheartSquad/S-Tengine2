#pragma once

#include <shader/ShaderComponentAlpha.h>
#include <shader/ShaderVariables.h>
#include <shader/ComponentShaderBase.h>
#include <GLUtils.h>
#include <RenderOptions.h>

ShaderComponentAlpha::ShaderComponentAlpha(ComponentShaderBase * _shader, float _alpha):
	ShaderComponent(_shader),
	alpha(_alpha),
	alphaLoc(-1)
{
}

ShaderComponentAlpha::~ShaderComponentAlpha(){
}
	
std::string ShaderComponentAlpha::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_ALPHA + ENDL;
}

std::string ShaderComponentAlpha::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_ALPHA + ENDL +
		"uniform float " + GL_UNIFORM_ID_ALPHA + SEMI_ENDL;
}

std::string ShaderComponentAlpha::getVertexBodyString(){
	return "";
}

std::string ShaderComponentAlpha::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentAlpha::getOutColorMod(){
	return
		"// multiply alpha" + ENDL +
		GL_OUT_OUT_COLOR + ".w *= "+ GL_UNIFORM_ID_ALPHA + SEMI_ENDL;

}

void ShaderComponentAlpha::load(){
	if(!loaded){
		alphaLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_ALPHA.c_str());
	}

	ShaderComponent::load();
}

void ShaderComponentAlpha::configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	glUniform1f(alphaLoc, alpha);
}

void ShaderComponentAlpha::setAlpha(float _alpha){
	alpha = _alpha;
	makeDirty();
}
float ShaderComponentAlpha::getAlpha(){
	return alpha;
}