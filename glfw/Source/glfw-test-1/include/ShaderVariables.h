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

#define MOD_GEO (bool hasGeo, std::string str) (hasGeo ? (GEO + str) : str)

const glm::mat4 BIAS_MATRIX(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0);

const std::string GEO							      = "geo";

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

const std::string UNIFORM							  = "uniform ";
const std::string OUT								  = "out ";
const std::string IN							      = "in ";

const std::string VEC4								  = "vec4 ";
const std::string VEC3								  = "vec3 ";
const std::string VEC2								  = "vec2 ";
const std::string MAT4								  = "mat4 ";
const std::string MAT3								  = "mat3 ";
const std::string FLOAT								  = "float ";
const std::string INT								  = "int ";

//Uniform variable names
const std::string GL_UNIFORM_ID_TEXTURE_SAMPLER       = "textureSampler";
const std::string GL_UNIFORM_ID_NUM_TEXTURES 		  = "numTextures";
const std::string GL_UNIFORM_ID_MODEL_MATRIX		  =	"model";
const std::string GL_UNIFORM_ID_MODEL_VIEW_PROJECTION = "MVP";
const std::string GL_UNIFORM_ID_NUM_LIGHTS			  = "numLights";
const std::string GL_UNIFORM_ID_LIGHTS_POSITION		  = "lights[].position";
const std::string GL_UNIFORM_ID_LIGHTS_INTENSITIES    =	"lights[].intensities";
const std::string GL_UNIFORM_ID_DEPTH_MVP	          =	"depthMVP";
const std::string GL_UNIFORM_ID_SHADOW_MAP_SAMPLER    = "shadowMapSampler";
const std::string GL_UNIFORM_ID_NUM_MATERIALS	      =	"numMaterials";
const std::string GL_UNIFORM_ID_MATERIAL_TYPE         = "materials[].materialType";
const std::string GL_UNIFORM_ID_LIGHTS_NO_ARRAY       = "lights";

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
const std::string SHADER_COMPONENT_SHADOW	          =	"SHADOW_COMPONENT";
const std::string SHADER_COMPONENT_LIGHT	          =	"LIGHT_COMPONENT";
const std::string SHADER_COMPONENT_PHONG			  = "SHADER_COMPONENT_PHONG";
const std::string SHADER_COMPONENT_MATERIAL			  = "SHADER_COMPONENT_MATERIAL";
const std::string SHADER_COMPONENT_AMBIENT			  = "SHADER_COMPONENT_AMBIENT";
const std::string SHADER_COMPONENT_DIFFUSE			  = "SHADER_COMPONENT_DIFFUSE";
const std::string SHADER_COMPONENT_BLINN			  = "SHADER_COMPONENT_BLINN";
const std::string SHADER_COMPONENT_VOXEL			  = "SHADER_COMPONENT_VOXEL";


const std::string SHADER_INCLUDE_LIGHT				  = "#ifndef " + SHADER_COMPONENT_LIGHT + "\n"
														"#define " + SHADER_COMPONENT_LIGHT + "\n"
														"struct Light{\n"
														"	vec3 position;\n"
														"	vec3 intensities;\n"
														"	float ambientCoefficient;\n"
														"	float attenuation;"
														"};\n"
														"uniform Light " + GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[ " + std::to_string(MAX_LIGHTS) + " ];\n"
														"uniform int " + GL_UNIFORM_ID_NUM_LIGHTS + ";\n"
														"#endif\n";

const std::string SHADER_INCLUDE_MATERIAL			  = "#ifndef " + SHADER_COMPONENT_MATERIAL + "\n"
														"#define " + SHADER_COMPONENT_MATERIAL + "\n"
														"struct Material{\n"
														"	float shininess;\n"
														"	vec3 specularColor;\n"
														"};\n"
														"uniform Material materials[5];\n"
														"uniform int numMaterials;\n"
														"#endif\n";