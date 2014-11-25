#pragma once

#include "TextureShaderComponent.h"

TextureShaderComponent::TextureShaderComponent() : ShaderComponent(){
}

TextureShaderComponent::~TextureShaderComponent(){
}

std::string TextureShaderComponent::getVertexVariablesString(){
	return "#define TEXTURE_COMPONENT";
}

std::string TextureShaderComponent::getFragmentVariablesString(){
	return 
		"#define TEXTURE_COMPONENT"
		"uniform sampler2D textureSampler[5];"
		"uniform int numTextures;";
}

std::string TextureShaderComponent::getVertexBodyString(){
	return "";
}

std::string TextureShaderComponent::getFragmentBodyString(){
	return
		"vec4 fragColorTex = vec4(0, 0, 0, 0);"
	
		"if(numTextures == 0){"
		"	fragColorTex = fragColor;"
		"}"

		"for(int i = 0; i < numTextures; i++){"
		"	if(i == 0){"
		"		fragColorTex = texture(textureSampler[i], fragUV).rgba;"
		"	}else{"
		"		fragColorTex = mix(fragColorTex, texture(textureSampler[i], fragUV).rgba, 0.5);"
		"	}"
		"}";
}

std::string TextureShaderComponent::getOutColorMod(){
	return "outColor = fragColorTex;";
}