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
	/*glm::vec3 lightInvDir = glm::vec3(0.5, 1, 1);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * _matrixStack->getModelMatrix();
	depthMVP = BIAS_MATRIX * depthMVP;*/
	glm::mat4 depthMVP = _matrixStack->getMVP();
	glUniformMatrix4fv(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_DEPTH_MVP.c_str()), 1, GL_FALSE, &depthMVP[0][0]);
}

std::string DepthMapShader::getVertString(){
	return 
		"#version 150\n"
		"#extension GL_ARB_explicit_attrib_location : enable\n"

		"layout(location = 1) in vec3 aVertexPosition;\n"

		"uniform mat4 depthMVP;\n"

		"void main(){\n"
			"gl_Position = depthMVP * vec4(aVertexPosition, 1.0);\n"
		"}\n";
}

std::string DepthMapShader::getFragString(){
	return	
	"#version 150\n"
	"#extension GL_ARB_explicit_attrib_location : enable\n"

	"layout(location = 0) out vec4 fragmentdepth;\n"

	"void main(){\n"

		"fragmentdepth = vec4(gl_FragCoord.z, 0, 0, 1);\n"
	"}\n";
}