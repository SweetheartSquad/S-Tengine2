#pragma once

#include <shader/ShaderComponentToon.h>
#include <sstream>
#include <shader/ShaderVariables.h>
#include <shader/ComponentShaderBase.h>
#include <MeshInterface.h>
#include <shader/SharedComponentShaderMethods.h>

ShaderComponentToon::ShaderComponentToon(ComponentShaderBase * _shader, Texture * _texture, bool _doubleSided) :
	ShaderComponentDiffuse(_shader, _doubleSided),
	levels(_texture->width), 
	texture(_texture), 
	levelsLoc(0), 
	numTexturesLoc(0), 
	textureLoc(0) 
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
	<< "uniform float " << GL_UNIFORM_ID_TOON_LEVELS << SEMI_ENDL
	<< "uniform sampler2D toonTexture" << SEMI_ENDL 
	<< ShaderComponentDiffuse::getFragmentVariablesString();
	return res.str();
}

std::string ShaderComponentToon::getVertexBodyString() {
	return "";
}

std::string ShaderComponentToon::getFragmentBodyString() {
	std::stringstream res;
	res << ShaderComponentDiffuse::getFragmentBodyString() << ENDL << 
	"const vec3 _lum = vec3(0.2125, 0.7154, 0.0721)" << SEMI_ENDL <<
	"float luminance = dot(outDiffuse, _lum)" << SEMI_ENDL <<
	"float level = round(luminance * " << GL_UNIFORM_ID_TOON_LEVELS << " - 1)" << SEMI_ENDL <<
	"level -= 1;"
	"if(level > " << GL_UNIFORM_ID_TOON_LEVELS << " - 1){level = " << GL_UNIFORM_ID_TOON_LEVELS << " - 1;}" << ENDL << 
	"if(level < 0){level = 0;}" << ENDL << 
	"vec3 toonMod = vec3(texelFetch(toonTexture, ivec2(level, 0), 0).rgb)"<< SEMI_ENDL;
	return res.str();
}

std::string ShaderComponentToon::getOutColorMod() {
	std::stringstream res;
	res << GL_OUT_OUT_COLOR << " *= " << "vec4(toonMod, 1)"<< SEMI_ENDL;
	return res.str();
}

void ShaderComponentToon::load() {
	if(!loaded){
		levelsLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_TOON_LEVELS.c_str());
		textureLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_TOON_TEXTURE.c_str());
	}
	texture->load();
	ShaderComponentDiffuse::load();
}

void ShaderComponentToon::unload() {
	if(loaded){
		levelsLoc = textureLoc = -1;
	}
	texture->unload();
	ShaderComponentDiffuse::unload();
}

void ShaderComponentToon::setTexture(Texture * _texture) {
	texture = _texture;
	levels  = _texture->width;
	makeDirty();
}

Texture * const ShaderComponentToon::getTexture() const {
	return texture;
}

void ShaderComponentToon::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	GLint curTex;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &curTex);
	glActiveTexture(curTex + 1);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);
	glUniform1i(textureLoc,  curTex - GL_TEXTURE0 + 1);
	glUniform1f(levelsLoc, static_cast<float>(levels));
	ShaderComponentDiffuse::configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}
