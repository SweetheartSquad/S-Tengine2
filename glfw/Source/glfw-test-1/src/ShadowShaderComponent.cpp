#pragma once

#include "ShadowShaderComponent.h"

ShadowShaderComponent::ShadowShaderComponent() : ShaderComponent(){
}

ShadowShaderComponent::~ShadowShaderComponent(){
}

std::string ShadowShaderComponent::getVertexVariablesString(){
	return 
		"#define SHADOW_COMPONENT"
		"out vec4 shadowCoord;";
}

std::string ShadowShaderComponent::getFragmentVariablesString(){
	return
		"#define SHADOW_COMPONENT"
		"uniform sampler2D shadowMapSampler;"
		"in vec4 shadowCoord;"
		"vec2 poissonDisk[16] = vec2[]("
		"glm::vec2( -0.94201624, -0.39906216 ),"
			"vec2( 0.94558609, -0.76890725 ),"
			"vec2( -0.094184101, -0.92938870 ),"
			"vec2( 0.34495938, 0.29387760 ),"
			"vec2( -0.91588581, 0.45771432 ),"
			"vec2( -0.81544232, -0.87912464 ),"
			"vec2( -0.38277543, 0.27676845 ),"
			"vec2( 0.97484398, 0.75648379 ),"
			"vec2( 0.44323325, -0.97511554 ),"
			"vec2( 0.53742981, -0.47373420 ),"
			"vec2( -0.26496911, -0.41893023 ),"
			"vec2( 0.79197514, 0.19090188 ),"
			"vec2( -0.24188840, 0.99706507 ),"
			"vec2( -0.81409955, 0.91437590 ),"
			"vec2( 0.19984126, 0.78641367 ),"
			"vec2( 0.14383161, -0.14100790 )"
		");";
}

std::string ShadowShaderComponent::getVertexBodyString(){
	return "shadowCoord = depthMVP * vec4(aVertexPosition, 1.0);";
}

std::string ShadowShaderComponent::getFragmentBodyString(){
	return 
		"float visibility = 1.0;"
		"for (int i=0; i<16; i++){"
			"vec3 ProjCoords = shadowCoord.xyz / shadowCoord.w;"
			"vec2 UVCoords"

			"UVCoords.x = (0.5 * ProjCoords.x + 0.5 + poissonDisk[i].x/900.0);"
			"UVCoords.y = (0.5 * ProjCoords.y + 0.5 + poissonDisk[i].y/900.0);"

			"float z = 0.5 * ProjCoords.z + 0.5;"
			"float Depth = texture(shadowMapSampler, UVCoords).x;"

			"if(Depth < z - 0.005){"
			"	visibility -= 0.05;"
			"}"
		"}"
		"#ifdef LIGHT_COMPONENT"
		"visibility += brightness;"
		"#endif";
}

std::string ShadowShaderComponent::getOutColorMod(){
	return "outColor *= vec4(clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1), clamp(visibility, 0.5, 1) , 1);";
}