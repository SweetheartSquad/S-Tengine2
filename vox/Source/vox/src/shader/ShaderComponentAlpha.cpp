#pragma once

#include <shader\ShaderComponentAlpha.h>
#include <shader\ShaderVariables.h>
#include <GLUtils.h>
#include <RenderOptions.h>
#include <shader\Shader.h>

#include <glew\glew.h>


ShaderComponentAlpha::ShaderComponentAlpha(Shader * _shader, float _alpha):
	ShaderComponent(_shader),
	alpha(_alpha)
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

void ShaderComponentAlpha::configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_ALPHA.c_str()), alpha);
}

void ShaderComponentAlpha::setAlpha(float _alpha){
	alpha = _alpha;
	makeDirty();
}
float ShaderComponentAlpha::getAlpha(){
	return alpha;
}