#pragma once 

#include "DiffuseShaderComponent.h"
#include "ShaderVariables.h"

DiffuseShaderComponent::DiffuseShaderComponent() : ShaderComponent(){
}

DiffuseShaderComponent::~DiffuseShaderComponent(){
}

std::string DiffuseShaderComponent::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_DIFFUSE + ENDL;
}

std::string DiffuseShaderComponent::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_DIFFUSE + ENDL +
		SHADER_INCLUDE_LIGHT;
}

std::string DiffuseShaderComponent::getVertexBodyString(){
	return EMPTY;
}

std::string DiffuseShaderComponent::getFragmentBodyString(){
	return
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + " && " + SHADER_COMPONENT_PHONG + ENDL + 
		"mat3 normalMatrix = transpose(inverse(mat3(model)))" + SEMI_ENDL +
		"vec3 normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
		"vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1))" + SEMI_ENDL +
		"float brightness = 0" + SEMI_ENDL +
		"vec3 outIntensities = vec3(0,0,0)" + SEMI_ENDL +

		"for(int i = 0; i < numLights; i++){" + ENDL +
			"vec3 surfaceToLight = lights[i].position - fragWorldPosition" + SEMI_ENDL +
			"brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal))" + SEMI_ENDL +
			"outIntensities += vec3(lights[i].intensities)" + SEMI_ENDL +
		"}" + ENDL +

		"brightness = clamp(brightness, 0.1, 1)" + SEMI_ENDL + 
		END_IF + ENDL;
}

std::string DiffuseShaderComponent::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + " && " + SHADER_COMPONENT_PHONG + ENDL + 
		GL_OUT_OUT_COLOR + " *= vec4(vec3(outIntensities), 1) * vec4(brightness, brightness, brightness, 1)" + SEMI_ENDL +
		END_IF + ENDL;
}