#pragma 

#include "TextureShaderComponent.h"
#include "ShaderVariables.h"

TextureShaderComponent::TextureShaderComponent() : ShaderComponent(){
}

TextureShaderComponent::~TextureShaderComponent(){
}

std::string TextureShaderComponent::getVertexVariablesString(){
	return "#define " + SHADER_COMPONENT_TEXTURE + "\n";
}

std::string TextureShaderComponent::getFragmentVariablesString(){
	return 
		"#define " + SHADER_COMPONENT_TEXTURE + "\n"
		"uniform sampler2D " + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[" + std::to_string(MAX_LIGHTS) + "]" + ";\n"
		"uniform int " + GL_UNIFORM_ID_NUM_TEXTURES + ";\n";
}

std::string TextureShaderComponent::getVertexBodyString(){
	return "";
}

std::string TextureShaderComponent::getFragmentBodyString(){
	return
		"vec4 fragColorTex = vec4(0, 0, 0, 0);\n"
	
		"if(" + GL_UNIFORM_ID_NUM_TEXTURES + " == 0){\n"
		"	\tfragColorTex = " + GL_IN_OUT_FRAG_COLOR + ";\n"
		"}\n"

		"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_TEXTURES + "; i++){"
		"	\tif(i == 0){\n"
		"		fragColorTex = texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba;\n"
		"	\t}else{\n"
		"		\t\tfragColorTex = mix(fragColorTex, texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba, 0.5);\n"
		"	\t}\n"
		"}\n";
}

std::string TextureShaderComponent::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " = fragColorTex;\n";
}