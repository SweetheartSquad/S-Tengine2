#pragma once 

#include <shader/ShaderComponentCubeMap.h>
#include <shader/ShaderVariables.h>
#include <shader/ComponentShaderBase.h>
#include <MatrixStack.h>

ShaderComponentCubeMap::ShaderComponentCubeMap(ComponentShaderBase * _shader) :
	ShaderComponentMVP(_shader),
	cubemapUniformLocation(-1)
{
}

ShaderComponentCubeMap::~ShaderComponentCubeMap() {
}

std::string ShaderComponentCubeMap::getFragmentVariablesString() {
	return 
		ShaderComponentMVP::getFragmentVariablesString() +
			"uniform samplerCube skybox" + SEMI_ENDL;
}

std::string ShaderComponentCubeMap::getVertexBodyString() {
	return TAB + "vec4 pos = " + GL_UNIFORM_ID_PROJECTION_MATRIX + " * " + GL_UNIFORM_ID_VIEW_MATRIX +  " * vec4(aVertexPosition, 1.0)" + SEMI_ENDL +
		"gl_Position = pos.xyww" + SEMI_ENDL;
}

std::string ShaderComponentCubeMap::getFragmentBodyString() {
	return "";
}

std::string ShaderComponentCubeMap::getOutColorMod() {
	// Not responsible for any colour modification
	return "outColor *= texture(skybox, fragVert)" + SEMI_ENDL;
}

void ShaderComponentCubeMap::load() {
	if(!loaded){
		cubemapUniformLocation	  = glGetUniformLocation(shader->getProgramId(), "skybox");
	}
	ShaderComponentMVP::load();
}

void ShaderComponentCubeMap::unload() {
	if(loaded){
		cubemapUniformLocation	  = -1;
	}
	ShaderComponentMVP::unload();
}

void ShaderComponentCubeMap::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	
	// trim the translation component out of the view matrix
	glm::mat4 viewOrig = *_matrixStack->getViewMatrix();
	glm::mat4 viewTrim = glm::mat4(glm::mat3(viewOrig));
	_matrixStack->setViewMatrix(&viewTrim);

	// configure MVP
	ShaderComponentMVP::configureUniforms(_matrixStack, _renderOption, _nodeRenderable);

	// configure cubemap
	glUniform1i(cubemapUniformLocation, 0);

	// reset the view matrix
	_matrixStack->setViewMatrix(&viewOrig);
}