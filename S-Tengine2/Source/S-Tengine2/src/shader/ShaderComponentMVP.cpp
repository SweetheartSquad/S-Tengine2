#pragma once 

#include <shader/ShaderComponentMVP.h>
#include <shader/ShaderVariables.h>
#include <shader/ComponentShaderBase.h>
#include <MatrixStack.h>

ShaderComponentMVP::ShaderComponentMVP(ComponentShaderBase * _shader) :
	ShaderComponent(_shader),
	modelUniformLocation(-1),
	viewUniformLocation(-1),
	projectionUniformLocation(-1),
	mvpUniformLocation(-1)
{
}

ShaderComponentMVP::~ShaderComponentMVP() {
}

std::string ShaderComponentMVP::getVertexVariablesString() {
	return 
			DEFINE + SHADER_COMPONENT_MVP + ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_VIEW_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + SEMI_ENDL;
}

std::string ShaderComponentMVP::getFragmentVariablesString() {
	return 
			DEFINE + SHADER_COMPONENT_MVP + ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_VIEW_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +
			"uniform mat4 " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + SEMI_ENDL;
}

std::string ShaderComponentMVP::getVertexBodyString() {
	return TAB + "gl_Position = " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + " * vec4(aVertexPosition, 1.0)" + SEMI_ENDL;
}

std::string ShaderComponentMVP::getFragmentBodyString() {
	return "";
}

std::string ShaderComponentMVP::getOutColorMod() {
	// Not responsible for any colour modification
	return "";
}

void ShaderComponentMVP::load() {
	if(!loaded){
		modelUniformLocation	  = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_MODEL_MATRIX.c_str());
		viewUniformLocation		  = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_VIEW_MATRIX.c_str());
		projectionUniformLocation = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_PROJECTION_MATRIX.c_str());
		mvpUniformLocation		  = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_MODEL_VIEW_PROJECTION.c_str());
	}
	ShaderComponent::load();
}

void ShaderComponentMVP::unload() {
	if(loaded){
		modelUniformLocation	  = -1;
		viewUniformLocation		  = -1;
		projectionUniformLocation = -1;
		mvpUniformLocation		  = -1;
	}
	ShaderComponent::unload();
}

void ShaderComponentMVP::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	
	const glm::mat4 * m = _matrixStack->getModelMatrix();
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
	
	m = _matrixStack->getViewMatrix();

	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
	
	m = _matrixStack->getProjectionMatrix();
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &(*m)[0][0]);

	m = _matrixStack->getMVP();
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
}
