#pragma once 

#include "shader/ShaderComponentPhong.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/SharedComponentShaderMethods.h"

ShaderComponentPhong::ShaderComponentPhong(ComponentShaderBase * _shader) :
	ShaderComponent(_shader)
{
}

ShaderComponentPhong::~ShaderComponentPhong(){
}

std::string ShaderComponentPhong::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_PHONG + ENDL;
}

std::string ShaderComponentPhong::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_PHONG + ENDL +
		SHADER_INCLUDE_LIGHT +
		SHADER_INCLUDE_MATERIAL;
}

std::string ShaderComponentPhong::getVertexBodyString(){
	return EMPTY;
}

std::string ShaderComponentPhong::getFragmentBodyString(){
	return 
	IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL + 
	"mat3 normalMatrix = transpose(inverse(mat3(" + GL_UNIFORM_ID_MODEL_MATRIX + ")))" + SEMI_ENDL +
	"vec3 normal = normalize(normalMatrix * fragNormal)" + SEMI_ENDL +
	"vec3 fragWorldPosition = vec3(" + GL_UNIFORM_ID_MODEL_MATRIX + " * vec4(fragVert, 1))" + SEMI_ENDL +
	"vec3 cameraPosition = -" + GL_UNIFORM_ID_VIEW_MATRIX + "[3].xyz * mat3(" + GL_UNIFORM_ID_VIEW_MATRIX + ")" + SEMI_ENDL +
	"vec3 surfaceToCamera = normalize(cameraPosition - fragWorldPosition)" + SEMI_ENDL +
	"vec3 outColorPhong = vec3(0)" + SEMI_ENDL +

	"vec3 surfaceToLight = vec3(0)" + SEMI_ENDL +
	"float attenuation = 1.0" + SEMI_ENDL +

	"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_LIGHTS + "; i++){" + ENDL +
		"for(int j = 0; j < " + GL_UNIFORM_ID_NUM_MATERIALS + "; j++){" + ENDL +

			SHADER_LIGHT_DISTANCE_AND_ATTENUATION +

			"vec3 ambient = " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].ambientCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			
			"//diffuse" + ENDL +
			"float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight))" + SEMI_ENDL +
			"vec3 diffuse = diffuseCoefficient * modFrag.rgb * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
		
			"//specular" + ENDL +
			"float specularCoefficient = 0.0" + SEMI_ENDL +
			"//only calculate specular for the front side of the surface" + ENDL +
			"if(diffuseCoefficient > 0.0){" + ENDL +
				"vec3 reflectDirection = reflect(-surfaceToLight, normal)" + SEMI_ENDL +
				"vec3 viewDirection = surfaceToCamera;" + ENDL +
				"specularCoefficient = pow(max(0.0, dot(reflectDirection, viewDirection)), materials[j].shininess)" + SEMI_ENDL +
			"}\n"
			"vec3 specular = specularCoefficient * materials[j].specularColor * " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[i].intensities" + SEMI_ENDL +
			"specular = clamp(specular, 0.0, 1.0);" + ENDL +

			"//linear color (color before gamma correction)" + ENDL +
			"vec3 linearColor = ambient + attenuation * (diffuse + specular)" + SEMI_ENDL +
    
			"//final color (after gamma correction)" + ENDL +
			"vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2)" + SEMI_ENDL +
			"vec3 gammaColor = pow(linearColor, gamma)" + SEMI_ENDL +
			"outColorPhong += gammaColor" + SEMI_ENDL +
		"}" + ENDL +
	"}" + ENDL +
	END_IF + ENDL;
}

std::string ShaderComponentPhong::getOutColorMod(){
	return 
		IF_NOT_DEFINED + SHADER_COMPONENT_BLINN + ENDL + 
		GL_OUT_OUT_COLOR + " *= vec4(outColorPhong, 1)" + SEMI_ENDL + 
		END_IF + ENDL;	
}

void ShaderComponentPhong::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentPhong::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	SharedComponentShaderMethods::configureLights(_matrixStack, _renderOption, _nodeRenderable);
	SharedComponentShaderMethods::configureMaterials(_matrixStack, _renderOption, _nodeRenderable);
}
