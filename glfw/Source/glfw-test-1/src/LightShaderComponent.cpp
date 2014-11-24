#pragma once 

#include "LightShaderComponent.h"

LightShaderComponent::LightShaderComponent() : ShaderComponent(){
}

LightShaderComponent::~LightShaderComponent(){
}

std::string LightShaderComponent::getVertexVariablesString(){
	return "#define LIGHT_COMPONENT";
}

std::string LightShaderComponent::getFragmentVariablesString(){
	return 
		"#define LIGHT_COMPONENT"
		"uniform mat4 model;"

		"struct Light{"
		"	vec3 position;"
		"	vec3 intensities;"
		"	float ambientCoefficient;"
		"	float attenuation;"
		"};"
		"uniform Light lights[5];"
		"uniform int numLights;";
}

std::string LightShaderComponent::getVertexBodyString(){
	return "";
}

std::string LightShaderComponent::getFragmentBodyString(){
	return 
		"mat3 normalMatrix = transpose(inverse(mat3(model)));"

		"vec3 normal = normalize(normalMatrix * fragNormal);"

		"vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));"

		"float brightness = 0;"
		"vec3 outIntensities = vec3(0,0,0);"

		"for(int i = 0; i < numLights; i++){"
			"vec3 surfaceToLight = lights[i].position - fragWorldPosition;"
			"brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));"
			"outIntensities += vec3(lights[i].intensities);"
		"}"

		"brightness = clamp(brightness, 0.1, 1);";
}

std::string LightShaderComponent::getOutColorMod(){
	return "outColor *= vec4(vec3(outIntensities), 1) * vec4(brightness, brightness, brightness, 1);";
}