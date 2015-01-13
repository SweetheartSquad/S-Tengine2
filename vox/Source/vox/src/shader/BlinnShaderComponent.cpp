#pragma once

#include "shader/BlinnShaderComponent.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/SharedComponentShaderMethods.h"

BlinnShaderComponent::BlinnShaderComponent() : ShaderComponent()
{
}

BlinnShaderComponent::~BlinnShaderComponent(){
}

std::string BlinnShaderComponent::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_BLINN + ENDL;
}

std::string BlinnShaderComponent::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_BLINN + ENDL 
		   + SHADER_INCLUDE_LIGHT
		   + SHADER_INCLUDE_MATERIAL;	
}

std::string BlinnShaderComponent::getVertexBodyString(){
	return EMPTY;
}

std::string BlinnShaderComponent::getFragmentBodyString(){
	return
	IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
	VAR_MAT3 + " normalMatrix = transpose(inverse(mat3(model)))" + SEMI_ENDL +
	VAR_VEC3 + " normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
	VAR_VEC3 + " fragWorldPosition = vec3(model * vec4(fragVert, 1))" + SEMI_ENDL +
	VAR_VEC3 + " surfaceToCamera = fragVert - fragWorldPosition" + SEMI_ENDL +
	
	VAR_VEC4 + " outColorBlinn = vec4(0,0,0,1)" + SEMI_ENDL +

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

			TAB + TAB + VAR_VEC3 + " ambient = lights[i].ambientCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
		
			TAB + TAB + "//diffuse" + ENDL +
			TAB + TAB + VAR_FLOAT + " diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))" + SEMI_ENDL +
			TAB + TAB + VAR_VEC3 + " diffuse = diffuseCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			TAB + TAB + "diffuse = clamp(diffuse, 0.0, 1.0)" + SEMI_ENDL +
		
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
			TAB + TAB + "//specular = clamp(specular, 0.0, 1.0)" + SEMI_ENDL +
			
			TAB + TAB + "//attenuation" + ENDL +
			TAB + TAB + VAR_FLOAT + " distanceToLight = length(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
			TAB + TAB + VAR_FLOAT + " attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2))" + SEMI_ENDL +
		
			TAB + TAB + "//linear color (color before gamma correction)" + ENDL +
			TAB + TAB + VAR_VEC3 + " linearColor = ambient + attenuation * (diffuse + specular)" + SEMI_ENDL +
    
			TAB + TAB + "//final color (after gamma correction)" + ENDL +
			TAB + TAB + VAR_VEC3 + " gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2)" + SEMI_ENDL +
			TAB + TAB + VAR_VEC3 + " gammaColor = pow(linearColor, gamma)" + SEMI_ENDL +
			TAB + TAB + "outColorBlinn = outColorBlinn + vec4(gammaColor, 1)" + SEMI_ENDL +
		TAB + "}" + ENDL +
	"}" + ENDL + 
	END_IF + ENDL;
}

std::string BlinnShaderComponent::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_PHONG + ENDL + 
		GL_OUT_OUT_COLOR + "*= outColorBlinn" + SEMI_ENDL + 
		END_IF + ENDL;
}

void BlinnShaderComponent::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
	SharedComponentShaderMethods::configureMaterials(_matrixStack, _renderOption, _nodeRenderable);
}
