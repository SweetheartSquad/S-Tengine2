#pragma once 

#include "LightShaderComponent.h"
#include "ShaderVariables.h"

LightShaderComponent::LightShaderComponent() : ShaderComponent(){
}

LightShaderComponent::~LightShaderComponent(){
}

std::string LightShaderComponent::getVertexVariablesString(){
	return "#define " + SHADER_COMPONENT_LIGHT + "\n";
}

std::string LightShaderComponent::getFragmentVariablesString(){
	return 
		"#define " + SHADER_COMPONENT_LIGHT + "\n"
		"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + ";\n"

		"struct Light{\n"
		"	vec3 position;\n"
		"	vec3 intensities;\n"
		"	float ambientCoefficient;\n"
		"	float attenuation;"
		"};\n"
		"uniform Light " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[ " + std::to_string(MAX_LIGHTS) + " ];\n"
		"uniform int " + GL_UNIFORM_ID_NUM_LIGHTS + ";\n";
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
	return GL_OUT_OUT_COLOR + " *= vec4(vec3(outIntensities), 1) * vec4(brightness, brightness, brightness, 1);\n";
}