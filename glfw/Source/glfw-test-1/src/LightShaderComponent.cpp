#pragma once 

#include "LightShaderComponent.h"

LightShaderComponent::LightShaderComponent() : ShaderComponent(){
}

LightShaderComponent::~LightShaderComponent(){
}

std::string LightShaderComponent::getVertexVariablesString(){
	return "#define LIGHT_COMPONENT\n";
}

std::string LightShaderComponent::getFragmentVariablesString(){
	return 
		"#define LIGHT_COMPONENT\n"
		"uniform mat4 model;\n"

		"struct Light{\n"
		"	vec3 position;\n"
		"	vec3 intensities;\n"
		"	float ambientCoefficient;\n"
		"	float attenuation;"
		"};\n"
		"uniform Light lights[5];\n"
		"uniform int numLights;\n";
}

std::string LightShaderComponent::getVertexBodyString(){
	return "";
}

std::string LightShaderComponent::getFragmentBodyString(){
	return 
		"mat3 normalMatrix = transpose(inverse(mat3(model)));\n"

		"vec3 normal = normalize(normalMatrix * fragNormal);\n"

		"vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));\n"

		"float brightness = 0;\n"
		"vec3 outIntensities = vec3(0,0,0);\n"

		"for(int i = 0; i < numLights; i++){"
			"vec3 surfaceToLight = lights[i].position - fragWorldPosition;\n"
			"brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));\n"
			"outIntensities += vec3(lights[i].intensities);\n"
		"}"

		"brightness = clamp(brightness, 0.1, 1);\n";
}

std::string LightShaderComponent::getOutColorMod(){
	return "outColor *= vec4(vec3(outIntensities), 1) * vec4(brightness, brightness, brightness, 1);\n";
}