#pragma once
 
#include <string>

#include "glm/glm.hpp"

/**********************************************************************
*
* A collection of constants used for interacting with shaders
* These variables help ensure consistency when creating new shaders
*
**********************************************************************/

#define MAX_LIGHTS    5
#define MAX_TEXTURES  5
#define MAX_MATERIALS 5

const glm::mat4 BIAS_MATRIX(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0);

const std::string GEO							      = "Geo";

const std::string DEFINE							  = "#define ";
const std::string IF_DEFINED						  = "#ifdef ";
const std::string IF_NOT_DEFINED				      = "#ifndef ";
const std::string END_IF							  = "#endif";
const std::string ELSE								  = "#else";

const std::string ENDL								  = "\n";
const std::string TAB								  = "\t";
const std::string SEMI								  = ";";
const std::string SEMI_ENDL							  = SEMI + ENDL;

const std::string EMPTY								  = "";

const std::string VAR_UNIFORM						  = "uniform ";
const std::string VAR_OUT							  = "out ";
const std::string VAR_IN							      = "in ";

const std::string VAR_VEC4							  = "vec4 ";
const std::string VAR_VEC3							  = "vec3 ";
const std::string VAR_VEC2						      = "vec2 ";
const std::string VAR_MAT4							  = "mat4 ";
const std::string VAR_MAT3							  = "mat3 ";
const std::string VAR_FLOAT						      = "float ";
const std::string VAR_INT							  = "int ";

//Uniform variable names
const std::string GL_UNIFORM_ID_MODEL_MATRIX		  =	"modelMat";
const std::string GL_UNIFORM_ID_VIEW_MATRIX			  =	"viewMat";
const std::string GL_UNIFORM_ID_PROJECTION_MATRIX     =	"projectionMat";
const std::string GL_UNIFORM_ID_MODEL_VIEW_PROJECTION = "MVP";
const std::string GL_UNIFORM_ID_VIEW_PROJECTION		  = "VP";

const std::string GL_UNIFORM_ID_DEPTH_OFFSET			  = "depthOffset";

const std::string GL_UNIFORM_ID_TEXTURE_SAMPLER       = "textureSampler";
const std::string GL_UNIFORM_ID_NUM_TEXTURES 		  = "numTextures";

const std::string GL_UNIFORM_ID_NUM_LIGHTS			  = "numLights";
const std::string GL_UNIFORM_ID_LIGHTS_NO_ARRAY       = "lights";
const std::string GL_UNIFORM_ID_LIGHTS_POSITION		  = GL_UNIFORM_ID_LIGHTS_NO_ARRAY+"[].position";
const std::string GL_UNIFORM_ID_LIGHTS_INTENSITIES    =	GL_UNIFORM_ID_LIGHTS_NO_ARRAY+"[].intensities";

const std::string GL_UNIFORM_ID_DEPTH_MVP	          =	"depthMVP";
const std::string GL_UNIFORM_ID_SHADOW_MAP_SAMPLER    = "shadowMapSampler";

const std::string GL_UNIFORM_ID_NUM_MATERIALS	      =	"numMaterials";
const std::string GL_UNIFORM_ID_MATERIALS_NO_ARRAY    = "materials";
const std::string GL_UNIFORM_ID_MATERIAL_TYPE         = GL_UNIFORM_ID_MATERIALS_NO_ARRAY + "[].materialType";

const std::string GL_UNIFORM_ID_TINT_RED			  = "tintRed";
const std::string GL_UNIFORM_ID_TINT_GREEN			  = "tintGreen";
const std::string GL_UNIFORM_ID_TINT_BLUE			  = "tintBlue";

const std::string GL_UNIFORM_ID_HUE					  = "hue";
const std::string GL_UNIFORM_ID_SATURATION			  = "saturation";
const std::string GL_UNIFORM_ID_VALUE				  = "value";

const std::string GL_UNIFORM_ID_ALPHA				  = "alpha";
const std::string GL_UNIFORM_ID_MASK_TEX			  = "maskTex";

const std::string GL_UNIFORM_ID_TEXT_COLOR			  = "textColor";


//Attribute variable names
const std::string GL_ATTRIBUTE_ID_VERTEX_POSITION	  = "aVertexPosition";
const std::string GL_ATTRIBUTE_ID_VERTEX_COLOR	      =	"aVertexColor";
const std::string GL_ATTRIBUTE_ID_VERTEX_NORMALS      =	"aVertexNormals";
const std::string GL_ATTRIBUTE_ID_VERTEX_UVS	      =	"aVertexUVs";

//In out variable names
const std::string GL_OUT_OUT_COLOR					  =	"outColor";
const std::string GL_IN_OUT_FRAG_COLOR				  =	"fragColor";
const std::string GL_IN_OUT_FRAG_UV				      =	"fragUV";
const std::string GL_IN_OUT_SHADOW_COORD			  =	"shadowCoord";

//Component names
const std::string SHADER_COMPONENT_TEXTURE	          =	"TEXTURE_COMPONENT";
const std::string SHADER_COMPONENT_MASK_RENDER	      =	"MASK_RENDER";
const std::string SHADER_COMPONENT_SHADOW	          =	"SHADOW_COMPONENT";
const std::string SHADER_COMPONENT_LIGHT	          =	"LIGHT_COMPONENT";
const std::string SHADER_COMPONENT_PHONG			  = "SHADER_COMPONENT_PHONG";
const std::string SHADER_COMPONENT_MATERIAL			  = "SHADER_COMPONENT_MATERIAL";
const std::string SHADER_COMPONENT_AMBIENT			  = "SHADER_COMPONENT_AMBIENT";
const std::string SHADER_COMPONENT_DIFFUSE			  = "SHADER_COMPONENT_DIFFUSE";
const std::string SHADER_COMPONENT_BLINN			  = "SHADER_COMPONENT_BLINN";
const std::string SHADER_COMPONENT_VOXEL			  = "SHADER_COMPONENT_VOXEL";
const std::string SHADER_COMPONENT_TINT				  = "SHADER_COMPONENT_TINT";
const std::string SHADER_COMPONENT_HSV				  = "SHADER_COMPONENT_HSV";
const std::string SHADER_COMPONENT_ALPHA			  = "SHADER_COMPONENT_ALPHA";
const std::string SHADER_COMPONENT_TEXT				  = "SHADER_COMPONENT_TEXT";
const std::string SHADER_COMPONENT_NORMAL			  = "SHADER_COMPONENT_NORMAL";
const std::string SHADER_COMPONENT_DEPTH			  = "SHADER_COMPONENT_DEPTH";
const std::string SHADER_COMPONENT_SSAO			      = "SHADER_COMPONENT_SSAO";
const std::string SHADER_COMPONENT_MASK			      = "SHADER_COMPONENT_MASK";
const std::string SHADER_COMPONENT_MVP			      = "SHADER_COMPONENT_MVP";
const std::string SHADER_COMPONENT_DEPTH_OFFSET		  = "SHADER_COMPONENT_DEPTH_OFFSET";


const std::string SHADER_INCLUDE_LIGHT				  = "#ifndef " + SHADER_COMPONENT_LIGHT + ENDL +
														"#define " + SHADER_COMPONENT_LIGHT + ENDL +
														"struct Light{\n"
														"	int type;\n"
														"	vec3 position;\n"
														"	vec3 intensities;\n"
														"	float ambientCoefficient;\n"
														"	float attenuation;\n"
														"	float cutoff;\n"
														"	float coneAngle;\n"
														"	vec3  coneDirection;\n"
														"};\n"
														"uniform Light " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[" + std::to_string(MAX_LIGHTS) + "]" + SEMI_ENDL +
														"uniform int " + GL_UNIFORM_ID_NUM_LIGHTS + SEMI_ENDL +
														"#endif\n";

// calculates the surfaceToLight vector and attenuation coefficient for lights[i]
const std::string SHADER_LIGHT_DISTANCE_AND_ATTENUATION = 
			"if(lights[i].type == 1){" + ENDL +
				TAB +"//DIRECTIONAL" + ENDL +
				TAB +"surfaceToLight = normalize(lights[i].position)" + SEMI_ENDL +
				TAB +"attenuation = lights[i].attenuation" + SEMI_ENDL +
			"} else {" + ENDL +	
				TAB + "//POINT" + ENDL +
				TAB + "surfaceToLight = normalize(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
				TAB + "//attenuation" + ENDL +
				TAB + "float distanceToLight = length(lights[i].position - fragWorldPosition)" + SEMI_ENDL +
				TAB + "if(lights[i].cutoff > 0){" + ENDL +
				TAB + TAB + "distanceToLight = min(distanceToLight, lights[i].cutoff)" + SEMI_ENDL +
				TAB + TAB + "attenuation = 1.0 - distanceToLight / lights[i].cutoff" + SEMI_ENDL +
				TAB + TAB + "attenuation = pow(lights[i].attenuation * attenuation, 2)" + SEMI_ENDL +
				TAB + "}else{" + ENDL +
				TAB + TAB + "attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2))" + SEMI_ENDL +
				//TAB + TAB + "attenuation = (attenuation - lights[i].cutoff) / (1 - lights[i].cutoff)" + SEMI_ENDL +
				//TAB + TAB + "attenuation = max(attenuation, 0)" + SEMI_ENDL +
				TAB + "}" + ENDL +
				// SpotLight
				TAB + "if(lights[i].type == 2){" + ENDL + 
				TAB + TAB + "float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(lights[i].coneDirection))))" + SEMI_ENDL +
				TAB + TAB + "if(lightToSurfaceAngle > lights[i].coneAngle){" + ENDL + 
				TAB + TAB + TAB + "attenuation = 0.0;" + ENDL + 
				TAB + TAB + "}" + ENDL + 
				TAB + "}" + ENDL + 
			"}";

const std::string SHADER_INCLUDE_MATERIAL			  = "#ifndef " + SHADER_COMPONENT_MATERIAL + "\n"
														"#define " + SHADER_COMPONENT_MATERIAL + "\n"
														"struct Material{\n"
														"	float shininess;\n"
														"	vec3 specularColor;\n"
														"};\n"
														"uniform Material " + GL_UNIFORM_ID_MATERIALS_NO_ARRAY + "[" + std::to_string(MAX_MATERIALS) + "]" + SEMI_ENDL +
														"uniform int " + GL_UNIFORM_ID_NUM_MATERIALS + SEMI_ENDL +
														"#endif\n";