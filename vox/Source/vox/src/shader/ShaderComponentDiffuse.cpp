#pragma once 

#include "shader/ShaderComponentDiffuse.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/SharedComponentShaderMethods.h"

ShaderComponentDiffuse::ShaderComponentDiffuse(Shader * _shader) :
	ShaderComponent(_shader)
{
}

ShaderComponentDiffuse::~ShaderComponentDiffuse(){
}

std::string ShaderComponentDiffuse::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_DIFFUSE + ENDL;
}

std::string ShaderComponentDiffuse::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_DIFFUSE + ENDL +
		SHADER_INCLUDE_LIGHT;
}

std::string ShaderComponentDiffuse::getVertexBodyString(){
	return EMPTY;
}

std::string ShaderComponentDiffuse::getFragmentBodyString(){
	return
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL +
		IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
		"mat3 normalMatrix = transpose(inverse(mat3(" + GL_UNIFORM_ID_MODEL_MATRIX + ")))" + SEMI_ENDL +
		"vec3 normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
		"vec3 fragWorldPosition = vec3(" + GL_UNIFORM_ID_MODEL_MATRIX + " * vec4(fragVert, 1))" + SEMI_ENDL +
		//"float brightness = 0" + SEMI_ENDL +
		"vec3 outDiffuse = vec3(0)" + SEMI_ENDL +
		"float attenuation = 1.0" + SEMI_ENDL +
		"vec3 surfaceToLight = vec3(0)" + SEMI_ENDL +

		"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_LIGHTS + "; i++){" + ENDL +
			
			SHADER_LIGHT_DISTANCE_AND_ATTENUATION +

			"vec3 ambient = vec3(lights[i].ambientCoefficient) * modFrag.rgb * lights[i].intensities" + SEMI_ENDL +
			"//diffuse" + ENDL +
			"float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))" + SEMI_ENDL +
			"vec3 diffuse = diffuseCoefficient * modFrag.rgb * lights[i].intensities" + SEMI_ENDL +

			
			"//linear color (color before gamma correction)" + ENDL +
			"vec3 linearColor = ambient + attenuation * (diffuse)" + SEMI_ENDL +
    
			"//final color (after gamma correction)" + ENDL +
			"vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2)" + SEMI_ENDL +
			"vec3 gammaColor = pow(linearColor, gamma)" + SEMI_ENDL +
			"outDiffuse += gammaColor" + SEMI_ENDL +

			/*
			"vec3 surfaceToLight = lights[i].position - fragWorldPosition" + SEMI_ENDL +
			"brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal))" + SEMI_ENDL +
			"outIntensities += vec3(lights[i].intensities)" + SEMI_ENDL +*/
		"}" + ENDL +

		//"brightness = clamp(brightness, 0.1, 1)" + SEMI_ENDL + 
		END_IF + ENDL +
		END_IF + ENDL;
}

std::string ShaderComponentDiffuse::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL +
		IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
		GL_OUT_OUT_COLOR + " *= vec4(outDiffuse, 1)" + SEMI_ENDL +
		END_IF + ENDL + 
		END_IF + ENDL;
}

void ShaderComponentDiffuse::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentDiffuse::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
}

