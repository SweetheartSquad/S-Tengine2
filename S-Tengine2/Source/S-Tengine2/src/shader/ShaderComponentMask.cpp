#pragma once

#include <shader\ShaderComponentMask.h>
#include <shader\ShaderVariables.h>
#include <shader\ComponentShaderBase.h>
#include <GLUtils.h>
#include <RenderOptions.h>
#include <Texture.h>

ShaderComponentMask::ShaderComponentMask(ComponentShaderBase * _shader, Texture * _maskTex) :
	ShaderComponent(_shader),
	maskTex(_maskTex),
	maskTexLoc(-1)
{
}

ShaderComponentMask::~ShaderComponentMask(){
}
	
std::string ShaderComponentMask::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK + ENDL;
}

std::string ShaderComponentMask::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_MASK + ENDL +
		"uniform sampler2D " + GL_UNIFORM_ID_MASK_TEX + SEMI_ENDL;
}

std::string ShaderComponentMask::getVertexBodyString(){
	return "";
}

std::string ShaderComponentMask::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentMask::getOutColorMod(){
	return
		"// discard transparent pixels in the mask" + ENDL +
		//"if("+ GL_UNIFORM_ID_MASK_TEX + " != 0){" + ENDL +
		GL_OUT_OUT_COLOR + ".a *= texture("+ GL_UNIFORM_ID_MASK_TEX + ", " + GL_IN_OUT_FRAG_UV + ").a" + SEMI_ENDL +
		/*"}" + */ENDL;
}

void ShaderComponentMask::load(){
	if(!loaded){
		maskTexLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_MASK_TEX.c_str());
	}

	ShaderComponent::load();
}

void ShaderComponentMask::configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	glActiveTexture(GL_TEXTURE0 + maskTex->textureId);
	glBindTexture(GL_TEXTURE_2D, maskTex->textureId);
	glUniform1i(maskTexLoc, maskTex != nullptr ? maskTex->textureId : 0);
}

void ShaderComponentMask::setMaskTex(Texture * _maskTex){
	maskTex = _maskTex;
	makeDirty();
}
Texture * ShaderComponentMask::getMaskTex(){
	return maskTex;
}