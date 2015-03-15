#pragma once 

#include "shader/PhongShaderComponent.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "RenderOptions.h"
#include "shader/SharedComponentShaderMethods.h"

PhongShaderComponent::PhongShaderComponent(Shader * _shader) :
	ShaderComponent(_shader)
{
}

PhongShaderComponent::~PhongShaderComponent(){
}

std::string PhongShaderComponent::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_PHONG + ENDL;
}

std::string PhongShaderComponent::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_PHONG + ENDL
		+ SHADER_INCLUDE_LIGHT
		+ SHADER_INCLUDE_MATERIAL;
}

std::string PhongShaderComponent::getVertexBodyString(){
	return EMPTY;
}

std::string PhongShaderComponent::getFragmentBodyString(){
	return 
	IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL + 
	"mat3 normalMatrix = transpose(inverse(mat3(model)))" + SEMI_ENDL +
	"vec3 normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
	"vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1))" + SEMI_ENDL +
	"vec3 surfaceToCamera = normalize(fragVert - fragWorldPosition)" + SEMI_ENDL +
	"vec4 outColorPhong = vec4(0,0,0,1)" + SEMI_ENDL +

	"vec3 surfaceToLight = vec3(0,0,0)" + SEMI_ENDL +
	"float attenuation = 1.0" + SEMI_ENDL +

	"for(int i = 0; i < numLights; i++){" + ENDL +
		"for(int j = 0; j < numMaterials; j++){" + ENDL +
			"if(lights[i].type == 1){" + ENDL +
				"//DIRECTIONAL" + ENDL +
				"surfaceToLight = normalize(lights[i].position)" + SEMI_ENDL +
				"attenuation = lights[i].attenuation" + SEMI_ENDL +
			"} else {" + ENDL +	
				"//POINT" + ENDL +
				"surfaceToLight = normalize(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
				"//attenuation" + ENDL +
				"float distanceToLight = length(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
				"attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2))" + SEMI_ENDL +
			"}" + ENDL +

			"vec3 ambient = lights[i].ambientCoefficient * modFrag.rgb * lights[i].intensities" + SEMI_ENDL +
			"//diffuse" + ENDL +
			"float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))" + SEMI_ENDL +
			"vec3 diffuse = diffuseCoefficient * modFrag.rgb * lights[i].intensities" + SEMI_ENDL +
		
			"//specular" + ENDL +
			"float specularCoefficient = 0.0" + SEMI_ENDL +
			"//only calculate specular for the front side of the surface" + ENDL +
			"if(diffuseCoefficient > 0.0){" + ENDL +
				"vec3 reflectDirection = reflect(-surfaceToLight, normal)" + SEMI_ENDL +
				"vec3 viewDirection = surfaceToCamera;" + ENDL +
				"specularCoefficient = pow(max(0.0,dot(reflectDirection, viewDirection)), materials[j].shininess)" + SEMI_ENDL +
			"}\n"
			"vec3 specular = specularCoefficient * materials[j].specularColor * lights[i].intensities" + SEMI_ENDL +
			"specular = clamp(specular, 0.0, 1.0);" + ENDL +

			"//attenuation" + ENDL +
			"float distanceToLight = length(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
			"float attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2))" + SEMI_ENDL +
		
			"//linear color (color before gamma correction)" + ENDL +
			"vec3 linearColor = ambient + attenuation * (diffuse + specular)" + SEMI_ENDL +
    
			"//final color (after gamma correction)" + ENDL +
			"vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2)" + SEMI_ENDL +
			"vec3 gammaColor = pow(linearColor, gamma)" + SEMI_ENDL +
			"outColorPhong = outColorPhong + vec4(gammaColor, 1)" + SEMI_ENDL +
		"}" + ENDL +
	"}" + ENDL +
	END_IF + ENDL;
}

std::string PhongShaderComponent::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL + 
		GL_OUT_OUT_COLOR + " *= outColorPhong;" + SEMI_ENDL + 
		END_IF + ENDL;	
}

void PhongShaderComponent::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
	SharedComponentShaderMethods::configureMaterials(_matrixStack, _renderOption, _nodeRenderable);
}
