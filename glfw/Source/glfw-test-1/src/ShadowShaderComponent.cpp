#pragma once

#include "ShadowShaderComponent.h"
#include <glm/detail/type_vec2.hpp>

ShadowShaderComponent::ShadowShaderComponent() : ShaderComponent(){
}

ShadowShaderComponent::~ShadowShaderComponent(){
}

std::string ShadowShaderComponent::getVertexVariablesString(){
	return "out vec4 shadowCoord;";
}

std::string ShadowShaderComponent::getFragmentVariablesString(){
	return "uniform sampler2D shadowMapSampler;"
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
	return "";
}

std::string ShadowShaderComponent::getFragmentBodyString(){
	return "";
}