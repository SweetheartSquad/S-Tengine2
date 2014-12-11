#pragma once

#include "ShadowShaderComponent.h"
#include "ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "MeshInterface.h"
#include "GLFWRenderOptions.h"

#include <glm/gtc/matrix_transform.inl>

#include "GL/glew.h"

class GLFWRenderOptions;

ShadowShaderComponent::ShadowShaderComponent() : ShaderComponent(){
}

ShadowShaderComponent::~ShadowShaderComponent(){
}

std::string ShadowShaderComponent::getVertexVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_SHADOW + ENDL + 
		IF_DEFINED + SHADER_COMPONENT_VOXEL + ENDL +
		"out vec4 " + GL_IN_OUT_SHADOW_COORD + "Geo" + SEMI_ENDL + 
		ELSE + ENDL +
		"out vec4 " + GL_IN_OUT_SHADOW_COORD + SEMI_ENDL + 
		END_IF + ENDL +
		"uniform mat4 " + GL_UNIFORM_ID_DEPTH_MVP + SEMI_ENDL;
}

std::string ShadowShaderComponent::getFragmentVariablesString(){
	return
		DEFINE + SHADER_COMPONENT_SHADOW + ENDL + 
		"uniform sampler2D " + GL_UNIFORM_ID_SHADOW_MAP_SAMPLER + SEMI_ENDL +
		"in vec4 " + GL_IN_OUT_SHADOW_COORD + SEMI_ENDL +
		"uniform mat4 " + GL_UNIFORM_ID_DEPTH_MVP + SEMI_ENDL +
		"vec2 poissonDisk[16] = vec2[](" + ENDL + 
		"vec2( -0.94201624, -0.39906216 )," + ENDL + 
			"vec2( 0.94558609, -0.76890725 )," + ENDL + 
			"vec2( -0.094184101, -0.92938870 )," + ENDL + 
			"vec2( 0.34495938, 0.29387760 )," + ENDL + 
			"vec2( -0.91588581, 0.45771432 )," + ENDL + 
			"vec2( -0.81544232, -0.87912464 )," + ENDL + 
			"vec2( -0.38277543, 0.27676845 )," + ENDL + 
			"vec2( 0.97484398, 0.75648379 )," + ENDL + 
			"vec2( 0.44323325, -0.97511554 )," + ENDL + 
			"vec2( 0.53742981, -0.47373420 )," + ENDL + 
			"vec2( -0.26496911, -0.41893023 )," + ENDL + 
			"vec2( 0.79197514, 0.19090188 )," + ENDL + 
			"vec2( -0.24188840, 0.99706507 )," + ENDL + 
			"vec2( -0.81409955, 0.91437590 )," + ENDL + 
			"vec2( 0.19984126, 0.78641367 )," + ENDL + 
			"vec2( 0.14383161, -0.14100790 )" + ENDL + 
		")" + SEMI_ENDL;
}

std::string ShadowShaderComponent::getVertexBodyString(){
	return 	
		IF_DEFINED + SHADER_COMPONENT_VOXEL + ENDL +
			GL_IN_OUT_SHADOW_COORD + "Geo" + " = " + GL_UNIFORM_ID_DEPTH_MVP + " * vec4(aVertexPosition, 1.0)" + SEMI_ENDL +
		ELSE + ENDL +
			GL_IN_OUT_SHADOW_COORD + " = " + GL_UNIFORM_ID_DEPTH_MVP + " * vec4(aVertexPosition, 1.0)" + SEMI_ENDL +
		END_IF + ENDL;
}

std::string ShadowShaderComponent::getFragmentBodyString(){
	return 
		"float visibility = 1.0" + SEMI_ENDL + 
		"for (int i=0; i<16; i++){" + ENDL + 
			"vec3 projCoords = " + GL_IN_OUT_SHADOW_COORD + ".xyz / " + GL_IN_OUT_SHADOW_COORD + ".w" + SEMI_ENDL + 
			"vec2 UVCoords" + SEMI_ENDL + 

			"UVCoords.x = (0.5 * projCoords.x + 0.5 + poissonDisk[i].x/900.0)" + SEMI_ENDL + 
			"UVCoords.y = (0.5 * projCoords.y + 0.5 + poissonDisk[i].y/900.0)" + SEMI_ENDL + 

			"float z = 0.5 * projCoords.z + 0.5" + SEMI_ENDL + 
			"float depth = texture(" + GL_UNIFORM_ID_SHADOW_MAP_SAMPLER + ", UVCoords).x" + SEMI_ENDL + 

			"if(depth < z - 0.005){" + ENDL + 
			"	visibility -= 0.05" + SEMI_ENDL + 
			"}"
		"}" + ENDL + 
		IF_DEFINED + SHADER_COMPONENT_AMBIENT + ENDL + 
		"visibility += brightness" + SEMI_ENDL + 
		END_IF + ENDL;
}

std::string ShadowShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " *= vec4(clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1) , 1)" + SEMI_ENDL;
}

void ShadowShaderComponent::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	
	if(mesh != nullptr){
		glm::vec3 lightInvDir = glm::vec3(0.5, 1, 1);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));

		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * _matrixStack->getCurrentMatrix();
		depthMVP = BIAS_MATRIX * depthMVP;
		glUniformMatrix4fv(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_DEPTH_MVP.c_str()), 1, GL_FALSE, &depthMVP[0][0]);

		if(static_cast<GLFWRenderOptions *>(_renderOption)->shadowMapTextureId != 0){
			glActiveTexture(GL_TEXTURE0 + mesh->textures.size());
			glBindTexture(GL_TEXTURE_2D, static_cast<GLFWRenderOptions *>(_renderOption)->shadowMapTextureId);
			glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_SHADOW_MAP_SAMPLER.c_str()), mesh->textures.size());
		}	
	}
}
