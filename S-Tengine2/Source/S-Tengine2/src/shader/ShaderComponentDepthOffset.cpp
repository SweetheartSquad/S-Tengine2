#pragma once

#include <shader/ShaderComponentDepthOffset.h>
#include <shader/ShaderVariables.h>
#include <shader/Shader.h>
#include <Sweet.h>

ShaderComponentDepthOffset::ShaderComponentDepthOffset(Shader * _shader) :
	ShaderComponent(_shader),
	depthOffset(0.0000000)
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
	if(currentCycle != sweet::step.cycles) {
		currentCycle = sweet::step.cycles;
		depthOffset = 0.000000000f;
	}else {
		depthOffset -= 0.00001f;
	}
	glUniform1f(glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_DEPTH_OFFSET.c_str()), depthOffset);
}