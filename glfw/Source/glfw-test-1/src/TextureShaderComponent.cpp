#pragma once

#include "TextureShaderComponent.h"

TextureShaderComponent::TextureShaderComponent() : ShaderComponent(){
}

TextureShaderComponent::~TextureShaderComponent(){
}

std::string TextureShaderComponent::getVertexVariablesString(){
	return "#define TEXTURE_COMPONENT\n";
}

std::string TextureShaderComponent::getFragmentVariablesString(){
	return 
		"#define TEXTURE_COMPONENT\n"
		"uniform sampler2D textureSampler[5];\n"
		"uniform int numTextures;\n";
}

std::string TextureShaderComponent::getVertexBodyString(){
	return "";
}

std::string TextureShaderComponent::getFragmentBodyString(){
	return
		"vec4 fragColorTex = vec4(0, 0, 0, 0);\n"
	
		"if(numTextures == 0){\n"
		"	fragColorTex = fragColor;\n"
		"}\n"

		"for(int i = 0; i < numTextures; i++){"
		"	if(i == 0){\n"
		"		fragColorTex = texture(textureSampler[i], fragUV).rgba;\n"
		"	}else{\n"
		"		fragColorTex = mix(fragColorTex, texture(textureSampler[i], fragUV).rgba, 0.5);\n"
		"	}\n"
		"}\n";
}

std::string TextureShaderComponent::getOutColorMod(){
	return "outColor = fragColorTex;\n";
}