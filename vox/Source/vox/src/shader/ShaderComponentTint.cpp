#pragma once

#include <shader\ShaderComponentTint.h>
#include <shader\ShaderVariables.h>
#include <GLUtils.h>
#include <RenderOptions.h>
#include <shader\Shader.h>

#include <glew\glew.h>


ShaderComponentTint::ShaderComponentTint(Shader * _shader, float _r, float _g, float _b):
	ShaderComponent(_shader),
	red(_r),
	green(_g),
	blue(_b)
{
}

ShaderComponentTint::~ShaderComponentTint(){
}
	
std::string ShaderComponentTint::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_TINT + ENDL;
}

std::string ShaderComponentTint::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_TINT + ENDL +
		"uniform float " + GL_UNIFORM_ID_TINT_RED + SEMI_ENDL + 
		"uniform float " + GL_UNIFORM_ID_TINT_GREEN + SEMI_ENDL +
		"uniform float " + GL_UNIFORM_ID_TINT_BLUE + SEMI_ENDL;
}

std::string ShaderComponentTint::getVertexBodyString(){
	return "";
}

std::string ShaderComponentTint::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentTint::getOutColorMod(){
	return
		"// modify rgb" + ENDL +
		GL_OUT_OUT_COLOR + ".r += " + GL_UNIFORM_ID_TINT_RED + SEMI_ENDL +
		GL_OUT_OUT_COLOR + ".g += " + GL_UNIFORM_ID_TINT_GREEN + SEMI_ENDL +
		GL_OUT_OUT_COLOR + ".b += " + GL_UNIFORM_ID_TINT_BLUE + SEMI_ENDL;
}

void ShaderComponentTint::configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_TINT_RED.c_str()), red);
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_TINT_GREEN.c_str()), green);
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_TINT_BLUE.c_str()), blue);
}

void ShaderComponentTint::setRed(float _r){
	red = _r;
	makeDirty();
}
void ShaderComponentTint::setGreen(float _g){
	green = _g;
	makeDirty();
}
void ShaderComponentTint::setBlue(float _b){
	blue = _b;
	makeDirty();
}
float ShaderComponentTint::getRed(){
	return red;
}
float ShaderComponentTint::getGreen(){
	return green;
}
float ShaderComponentTint::getBlue(){
	return blue;
}