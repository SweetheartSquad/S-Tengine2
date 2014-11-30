#pragma once

#include "ShadowShaderComponent.h"
#include "ShaderVariables.h"

ShadowShaderComponent::ShadowShaderComponent() : ShaderComponent(){
}

ShadowShaderComponent::~ShadowShaderComponent(){
}

std::string ShadowShaderComponent::getVertexVariablesString(){
	return 
		"#define " + SHADER_COMPONENT_SHADOW + "\n"
		"out vec4 " + GL_IN_OUT_SHADOW_COORD + ";\n"
		"uniform mat4 " + GL_UNIFORM_ID_DEPTH_MVP + ";\n";
}

std::string ShadowShaderComponent::getFragmentVariablesString(){
	return
		"#define " + SHADER_COMPONENT_SHADOW + " \n"
		"uniform sampler2D " + GL_UNIFORM_ID_SHADOW_MAP_SAMPLER + ";\n"
		"in vec4 " + GL_IN_OUT_SHADOW_COORD + ";\n"
		"uniform mat4 " + GL_UNIFORM_ID_DEPTH_MVP + ";\n"
		"vec2 poissonDisk[16] = vec2[](\n"
		"vec2( -0.94201624, -0.39906216 ),\n"
			"vec2( 0.94558609, -0.76890725 ),\n"
			"vec2( -0.094184101, -0.92938870 ),\n"
			"vec2( 0.34495938, 0.29387760 ),\n"
			"vec2( -0.91588581, 0.45771432 ),\n"
			"vec2( -0.81544232, -0.87912464 ),\n"
			"vec2( -0.38277543, 0.27676845 ),\n"
			"vec2( 0.97484398, 0.75648379 ),\n"
			"vec2( 0.44323325, -0.97511554 ),\n"
			"vec2( 0.53742981, -0.47373420 ),\n"
			"vec2( -0.26496911, -0.41893023 ),\n"
			"vec2( 0.79197514, 0.19090188 ),\n"
			"vec2( -0.24188840, 0.99706507 ),\n"
			"vec2( -0.81409955, 0.91437590 ),\n"
			"vec2( 0.19984126, 0.78641367 ),\n"
			"vec2( 0.14383161, -0.14100790 )\n"
		");\n";
}

std::string ShadowShaderComponent::getVertexBodyString(){
	return GL_IN_OUT_SHADOW_COORD + " = " + GL_UNIFORM_ID_DEPTH_MVP + " * vec4(aVertexPosition, 1.0);\n";
}

std::string ShadowShaderComponent::getFragmentBodyString(){
	return 
		"float visibility = 1.0;\n"
		"for (int i=0; i<16; i++){\n"
			"vec3 projCoords = " + GL_IN_OUT_SHADOW_COORD + ".xyz / " + GL_IN_OUT_SHADOW_COORD + ".w;\n"
			"vec2 UVCoords;\n"

			"UVCoords.x = (0.5 * projCoords.x + 0.5 + poissonDisk[i].x/900.0);\n"
			"UVCoords.y = (0.5 * projCoords.y + 0.5 + poissonDisk[i].y/900.0);\n"

			"float z = 0.5 * projCoords.z + 0.5;\n"
			"float depth = texture(" + GL_UNIFORM_ID_SHADOW_MAP_SAMPLER + ", UVCoords).x;\n"

			"if(depth < z - 0.005){\n"
			"	visibility -= 0.05;\n"
			"}"
		"}\n"
		"#ifdef " + SHADER_COMPONENT_LIGHT + "\n"
		"visibility += brightness;\n"
		"#endif\n";
}

std::string ShadowShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " *= vec4(clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1) , 1);\n";
}