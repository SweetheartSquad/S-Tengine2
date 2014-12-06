#pragma once 

#include "PhongShaderComponent.h"
#include "ShaderVariables.h"

PhongShaderComponent::PhongShaderComponent() : ShaderComponent(){
}

PhongShaderComponent::~PhongShaderComponent(){
}

std::string PhongShaderComponent::getVertexVariablesString(){
	return "#define " + SHADER_COMPONENT_PHONG + "\n";
}

std::string PhongShaderComponent::getFragmentVariablesString(){
	return 
		"#define " + SHADER_COMPONENT_PHONG + "\n"
		"#ifndef " + SHADER_COMPONENT_MATERIAL + "\n"
		"struct Material{\n"
			"\tfloat shininess;\n"
			"\tvec3 specularColor;\n"
		"};\n"
		"uniform Material materials[5];\n"
		"uniform int numMaterials;\n"
		"#endif\n"
		"#ifndef " + SHADER_COMPONENT_LIGHT + "\n"
		"struct Light{\n"
		"	vec3 position;\n"
		"	vec3 intensities;\n"
		"	float ambientCoefficient;\n"
		"	float attenuation;"
		"};\n"
		"uniform Light " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[ " + std::to_string(MAX_LIGHTS) + " ];\n"
		"uniform int " + GL_UNIFORM_ID_NUM_LIGHTS + ";\n";
		"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + ";\n"
		"#endif\n";

}

std::string PhongShaderComponent::getVertexBodyString(){
	return "";
}

std::string PhongShaderComponent::getFragmentBodyString(){
	return 
	"mat3 normalMatrix = transpose(inverse(mat3(model)));\n"
	"vec3 normal = normalize(normalMatrix * fragNormal);\n"
	"vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));\n"
	"vec3 surfaceToCamera = normalize(fragVert - fragWorldPosition);\n"
	"vec4 outColorTemp = vec4(0,0,0,1);\n"
	"for(int i = 0; i < numLights; i++){\n"
		"for(int j = 0; j < numMaterials; j++){\n"
			"vec3 surfaceToLight = normalize(lights[i].position - fragWorldPosition);\n"
			"//ambient\n"
			"vec3 ambient = lights[i].ambientCoefficient * fragColor.rgb * lights[i].intensities;\n"
		
			"//diffuse\n"
			"float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));\n"
			"vec3 diffuse = diffuseCoefficient * fragColor.rgb * lights[i].intensities;\n"
		
			"//specular\n"
			"float specularCoefficient = 0.0;\n"
			"//only calculate specular for the front side of the surface\n"
			"if(diffuseCoefficient > 0.0){\n"
				"vec3 reflectDirection = reflect(-surfaceToLight, normal);\n"
				"vec3 viewDirection = surfaceToCamera;\n"
				"specularCoefficient = pow(max(0.0,dot(reflectDirection, viewDirection)), materials[j].shininess);\n"
			"}\n"
			"vec3 specular = specularCoefficient * materials[j].specularColor * lights[i].intensities;\n"
			"specular = clamp(specular, 0.0, 1.0);\n"

			"//attenuation\n"
			"float distanceToLight = length(lights[i].position - fragWorldPosition);\n"
			"float attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2));\n"
		
			"//linear color (color before gamma correction)\n"
			"vec3 linearColor = ambient + attenuation * (diffuse + specular);\n"
    
			"//final color (after gamma correction)\n"
			"vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
			"vec3 gammaColor = pow(linearColor, gamma);\n"
			"outColorTemp = outColorTemp + vec4(gammaColor, 1);\n"
		"}\n"
	"}\n";
}

std::string PhongShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " *= outColorTemp;\n";
}
