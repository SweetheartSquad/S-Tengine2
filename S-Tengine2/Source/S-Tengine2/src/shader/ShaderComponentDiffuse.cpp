#pragma once 

#include "shader/ShaderComponentDiffuse.h"
#include "shader/ShaderVariables.h"
#include "shader/SharedComponentShaderMethods.h"
#include <shader/ComponentShaderBase.h>
#include "MatrixStack.h"
#include "RenderOptions.h"

ShaderComponentDiffuse::ShaderComponentDiffuse(ComponentShaderBase * _shader, bool _doubleSided) :
	ShaderComponent(_shader),
	doubleSided(_doubleSided)
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

		"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_LIGHTS + "; ++i){" + ENDL +
			
			SHADER_LIGHT_DISTANCE_AND_ATTENUATION +

			"vec3 ambient = vec3(" + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].ambientCoefficient) * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			"//diffuse" + ENDL +
			(doubleSided ? "float diffuseCoefficient = abs(dot(normal, surfaceToLight))" : "float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))") + SEMI_ENDL +
			"vec3 diffuse = diffuseCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +

			
			"//linear color (color before gamma correction)" + ENDL +
			"vec3 linearColor = ambient + (attenuation * (diffuse))" + SEMI_ENDL +
    
			"//final color (after gamma correction)" + ENDL +
			"vec3 gamma = vec3(0.4545454)" + SEMI_ENDL + // 1.0/2.2 = 0.45454545454545454545454545454545
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

void ShaderComponentDiffuse::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentDiffuse::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
}