#pragma once

//Uniform variable names
#define GL_UNIFORM_ID_TEXTURE_SAMPLER		"textureSampler"
#define GL_UNIFORM_ID_NUM_TEXTURES			"numTextures"
#define GL_UNIFORM_ID_MODEL_MATRIX			"model"
#define GL_UNIFORM_ID_MODEL_VIEW_PROJECTION "MVP"
#define GL_UNIFORM_ID_NUM_LIGHTS			"numLights"
#define GL_UNIFORM_ID_LIGHTS_POSITION		"lights[].position"
#define GL_UNIFORM_ID_LIGHTS_INTENSITIES	"lights[].intensities"
#define GL_UNIFORM_ID_DEPTH_MVP				"depthMVP"
#define GL_UNIFORM_ID_SHADOW_MAP_SAMPLER    "shadowMapSampler"
#define GL_UNIFORM_ID_NUM_MATERIALS			"numMaterials"
#define GL_UNIFORM_ID_MATERIAL_TYPE			"materials[].materialType"

//Attribute variable names
#define GL_ATTRIBUTE_ID_VERTEX_POSITION		"aVertexPosition"
#define GL_ATTRIBUTE_ID_VERTEX_COLOR		"aVertexColor"
#define GL_ATTRIBUTE_ID_VERTEX_NORMALS		"aVertexNormals"
#define GL_ATTRIBUTE_ID_VERTEX_UVS			"aVertexUVs"