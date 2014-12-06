#pragma once

#include <string>

#define MAX_LIGHTS    5
#define MAX_TEXTURES  5
#define MAX_MATERIALS 5

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