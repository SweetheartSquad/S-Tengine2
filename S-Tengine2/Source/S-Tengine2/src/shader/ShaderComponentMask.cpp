#pragma once

#include <shader/ShaderComponentMask.h>
#include <shader/ShaderVariables.h>
#include <shader/ShaderComponentDepth.h>

ShaderComponentMask::ShaderComponentMask(Shader * _shader) :
	ShaderComponent(_shader),
	maskTextureId(0)
{
}

void ShaderComponentMask::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	// Do nothing
}

std::string ShaderComponentMask::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK + ENDL;
}

std::string ShaderComponentMask::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK + ENDL;
}

std::string ShaderComponentMask::getVertexBodyString(){
	return "";
}


std::string ShaderComponentMask::getFragmentBodyString(){
	return "uniform sampler2D maskTextureSampler" + SEMI_ENDL;
}

std::string ShaderComponentMask::getOutColorMod(){
	return 
		"vec 4 maskTexColor = texture(maskTextureSampler, " + GL_IN_OUT_FRAG_UV + ")" + SEMI_ENDL + 
		GL_OUT_OUT_COLOR  + ".a *= maskTexColor.r";
}

void ShaderComponentMask::setMaskTextureId(GLuint _id){
	maskTextureId = _id;
	makeDirty();
}

void ShaderComponentMask::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
}
