#pragma 

#include "TextureShaderComponent.h"
#include "ShaderVariables.h"

TextureShaderComponent::TextureShaderComponent() : ShaderComponent(){
}

TextureShaderComponent::~TextureShaderComponent(){
}

std::string TextureShaderComponent::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_TEXTURE + ENDL;
}

std::string TextureShaderComponent::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_TEXTURE + ENDL + 
		"uniform sampler2D " + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[" + std::to_string(MAX_LIGHTS) + "]" + SEMI_ENDL + 
		"uniform int " + GL_UNIFORM_ID_NUM_TEXTURES + SEMI_ENDL;
}

std::string TextureShaderComponent::getVertexBodyString(){
	return "";
}

std::string TextureShaderComponent::getFragmentBodyString(){
	return
		"vec4 fragColorTex = vec4(0, 0, 0, 0);" + SEMI_ENDL + 
	
		"if(" + GL_UNIFORM_ID_NUM_TEXTURES + " == 0){" + ENDL + 
		"	fragColorTex = " + GL_IN_OUT_FRAG_COLOR + SEMI_ENDL + 
		"}" + ENDL + 

		"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_TEXTURES + "; i++){" + ENDL + 
		"	if(i == 0){" + ENDL + 
		"		fragColorTex = texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba" + SEMI_ENDL + 
		"	}else{" + ENDL + 
		"		fragColorTex = mix(fragColorTex, texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba, 0.5)" + SEMI_ENDL + 
		"	}" + ENDL + 
		"}" + ENDL;
}

std::string TextureShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " = fragColorTex" + SEMI_ENDL;
}