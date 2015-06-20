#pragma once

#include "MatrixStack.h"
#include "shader/ShaderVariables.h"
#include "shader/DepthMapShader.h"
#include "RenderOptions.h"

#include "GL/glew.h"

#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.inl>


DepthMapShader::DepthMapShader(bool _autoRelease) :
	Shader(getVertString(), getFragString(), _autoRelease)
{
}

void DepthMapShader::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	const glm::mat4 * depthMVP = _matrixStack->getVP();
	glUniformMatrix4fv(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_DEPTH_MVP.c_str()), 1, GL_FALSE, &(*depthMVP)[0][0]);
}

std::string DepthMapShader::getVertString(){
	return 
		"#version 150\n"

		"in vec3 " + GL_ATTRIBUTE_ID_VERTEX_POSITION + SEMI_ENDL +

		"uniform mat4 depthMVP;\n"

		"void main(){\n"
			"gl_Position = depthMVP * vec4(aVertexPosition, 1.0);\n"
		"}\n";
}

std::string DepthMapShader::getFragString(){
	return	
		"#version 150\n"

		"in vec3 " + GL_ATTRIBUTE_ID_VERTEX_POSITION + SEMI_ENDL +

		"out vec4 fragmentDepth" + SEMI_ENDL + 

		"void main(){\n"
			"float zbuffer = gl_FragCoord.w;\n"
			"fragmentDepth = vec4(zbuffer, zbuffer, zbuffer, 1);\n"
		"}\n";
}