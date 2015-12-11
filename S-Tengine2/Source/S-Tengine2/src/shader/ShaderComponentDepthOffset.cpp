#pragma once

#include <shader/ShaderComponentDepthOffset.h>
#include <shader/ShaderVariables.h>
#include <shader/Shader.h>
#include <Sweet.h>

float sweet::depthOffset = 0.0000000;
unsigned long long sweet::currentCycle = 0;

ShaderComponentDepthOffset::ShaderComponentDepthOffset(Shader * _shader) :
	ShaderComponent(_shader)
{
}

ShaderComponentDepthOffset::~ShaderComponentDepthOffset() {
}

std::string ShaderComponentDepthOffset::getVertexVariablesString() {
	return "";
}

std::string ShaderComponentDepthOffset::getFragmentVariablesString() {
	return 
			DEFINE + SHADER_COMPONENT_DEPTH_OFFSET + ENDL +
			"uniform float " + GL_UNIFORM_ID_DEPTH_OFFSET + SEMI_ENDL;
}

std::string ShaderComponentDepthOffset::getVertexBodyString() {
	return "";
}

std::string ShaderComponentDepthOffset::getFragmentBodyString() {
	return "";
}

std::string ShaderComponentDepthOffset::getOutColorMod() {
	return "gl_FragDepth = gl_FragCoord.z + " + GL_UNIFORM_ID_DEPTH_OFFSET + SEMI_ENDL;
}

void ShaderComponentDepthOffset::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	if(sweet::currentCycle != sweet::step.cycles) {
		sweet::currentCycle = sweet::step.cycles;
		sweet::depthOffset = 0.000000000f;
	}else {
		sweet::depthOffset -= 0.00001f;
	}
	glUniform1f(glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_DEPTH_OFFSET.c_str()), sweet::depthOffset);
}