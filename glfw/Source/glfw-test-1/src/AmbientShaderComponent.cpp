#pragma once

#include "ShaderVariables.h"
#include "AmbientShaderComponent.h"

AmbientShaderComponent::AmbientShaderComponent() : ShaderComponent(){
}

AmbientShaderComponent::~AmbientShaderComponent(){
}

std::string AmbientShaderComponent::getVertexVariablesString(){
	return 
		"#define " + SHADER_COMPONENT_AMBIENT + "\n";
}

std::string AmbientShaderComponent::getFragmentVariablesString(){
	return
		"#define " + SHADER_COMPONENT_AMBIENT + " \n";
}

std::string AmbientShaderComponent::getVertexBodyString(){
	return "";
}

std::string AmbientShaderComponent::getFragmentBodyString(){
	return "";
}

std::string AmbientShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + "";
}