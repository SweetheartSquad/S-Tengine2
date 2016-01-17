#pragma once

#include <shader/ShaderComponentToon.h>
#include <sstream>
#include <shader/ShaderVariables.h>
#include <shader/Shader.h>

ShaderComponentToon::ShaderComponentToon(Shader* _shader, int _levels) :
	ShaderComponent(_shader),
	levels(_levels)
{
}

ShaderComponentToon::~ShaderComponentToon() {
}

std::string ShaderComponentToon::getVertexVariablesString() {
	return "";
}

std::string ShaderComponentToon::getFragmentVariablesString() {
	std::stringstream res;
	res << DEFINE << SHADER_COMPONENT_TOON << ENDL
	<< "uniform float " << GL_UNIFORM_ID_TOON_LEVELS << SEMI_ENDL;
	return res.str();
}

std::string ShaderComponentToon::getVertexBodyString() {
	return "";
}

std::string ShaderComponentToon::getFragmentBodyString() {
	return "";
}

std::string ShaderComponentToon::getOutColorMod() {
	std::stringstream res;
	res << "vec3 _lum = vec3(0.2125, 0.7154, 0.0721)" << SEMI_ENDL <<
	"float luminance = dot(" << GL_OUT_OUT_COLOR << ".rgb, _lum)" << SEMI_ENDL <<
	"float level = floor(luminance * " << GL_UNIFORM_ID_TOON_LEVELS << ")" << SEMI_ENDL <<
	"float toon = level / " << GL_UNIFORM_ID_TOON_LEVELS << SEMI_ENDL;
	res << GL_OUT_OUT_COLOR << ".rgb *= toon" << SEMI_ENDL;
	return res.str();
}

void ShaderComponentToon::load() {
	if(!loaded){
		levelsLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_TOON_LEVELS.c_str());
	}
	ShaderComponent::load();
}

void ShaderComponentToon::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	glUniform1f(levelsLoc, (float)levels);
}


void ShaderComponentToon::setLevels(int _levels) {
	levels = _levels;
	makeDirty();
}

int ShaderComponentToon::getLevels() const {
	return levels;
}