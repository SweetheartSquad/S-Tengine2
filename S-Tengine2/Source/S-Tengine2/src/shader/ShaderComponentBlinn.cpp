#pragma once

#include "shader/ShaderComponentBlinn.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/SharedComponentShaderMethods.h"

ShaderComponentBlinn::ShaderComponentBlinn(Shader * _shader) :
	ShaderComponent(_shader)
{
}

ShaderComponentBlinn::~ShaderComponentBlinn(){
}

std::string ShaderComponentBlinn::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_BLINN + ENDL;
}

std::string ShaderComponentBlinn::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_BLINN + ENDL +
		SHADER_INCLUDE_LIGHT +
		SHADER_INCLUDE_MATERIAL;	
}

std::string ShaderComponentBlinn::getVertexBodyString(){
	return EMPTY;
}

std::string ShaderComponentBlinn::getFragmentBodyString(){
	return
	IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
	VAR_MAT3 + " normalMatrix = transpose(inverse(mat3(" + GL_UNIFORM_ID_MODEL_MATRIX + ")))" + SEMI_ENDL +
	VAR_VEC3 + " normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
	VAR_VEC3 + " fragWorldPosition = vec3(" + GL_UNIFORM_ID_MODEL_MATRIX + " * vec4(fragVert, 1))" + SEMI_ENDL +
	VAR_VEC3 + " cameraPosition = -" + GL_UNIFORM_ID_VIEW_MATRIX + "[3].xyz * mat3(" + GL_UNIFORM_ID_VIEW_MATRIX + ")" + SEMI_ENDL +
	VAR_VEC3 + " surfaceToCamera = normalize(cameraPosition - fragWorldPosition)" + SEMI_ENDL +
	VAR_VEC3 + " outColorBlinn = vec3(0)" + SEMI_ENDL +

	"vec3 surfaceToLight = vec3(0)" + SEMI_ENDL +
	"float attenuation = 1.0" + SEMI_ENDL +

	"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_LIGHTS + "; i++){" + ENDL +
		"for(int j = 0; j < " + GL_UNIFORM_ID_NUM_MATERIALS + "; j++){" + ENDL +
			
			SHADER_LIGHT_DISTANCE_AND_ATTENUATION +

			TAB + TAB + VAR_VEC3 + " ambient = lights[i].ambientCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			
			TAB + TAB + "//diffuse" + ENDL +
			TAB + TAB + VAR_FLOAT + " diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))" + SEMI_ENDL +
			TAB + TAB + VAR_VEC3 + " diffuse = diffuseCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			
			TAB + TAB + "//specular" + ENDL +
			TAB + TAB + VAR_FLOAT + " specularCoefficient = 0.0" + SEMI_ENDL +
			TAB + TAB + "//only calculate specular for the front side of the surface" + ENDL +
			TAB + TAB + "if(diffuseCoefficient > 0.0){" + ENDL +
				TAB + TAB + TAB + VAR_VEC3 + " lightDirection = normalize(-surfaceToLight)" + SEMI_ENDL +
				TAB + TAB + TAB + VAR_VEC3 + " viewDirection = normalize(-surfaceToCamera)" + SEMI_ENDL +
				TAB + TAB + TAB + VAR_VEC3 + " halfAngle = normalize(lightDirection + viewDirection)" + SEMI_ENDL +

				TAB + TAB + TAB + "specularCoefficient = pow(max(0.0, dot(halfAngle, normal)), materials[j].shininess)" + SEMI_ENDL +
			TAB + TAB +"}" + ENDL +
			TAB + TAB + VAR_VEC3 + " specular = specularCoefficient * materials[j].specularColor * lights[i].intensities" + SEMI_ENDL +
			TAB + TAB + "specular = clamp(specular, 0.0, 1.0)" + SEMI_ENDL +
			
			TAB + TAB + "//linear color (color before gamma correction)" + ENDL +
			TAB + TAB + VAR_VEC3 + " linearColor = ambient + attenuation * (diffuse + specular)" + SEMI_ENDL +
    
			TAB + TAB + "//final color (after gamma correction)" + ENDL +
			TAB + TAB + VAR_VEC3 + " gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2)" + SEMI_ENDL +
			TAB + TAB + VAR_VEC3 + " gammaColor = pow(linearColor, gamma)" + SEMI_ENDL +
			TAB + TAB + "outColorBlinn += gammaColor" + SEMI_ENDL +
		TAB + "}" + ENDL +
	"}" + ENDL + 
	END_IF + ENDL;
}

std::string ShaderComponentBlinn::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
		GL_OUT_OUT_COLOR + " *= vec4(outColorBlinn, 1)" + SEMI_ENDL + 
		END_IF + ENDL;
}

void ShaderComponentBlinn::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentBlinn::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
	SharedComponentShaderMethods::configureMaterials(_matrixStack, _renderOption, _nodeRenderable);
}
