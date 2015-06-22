#pragma once

#include <shader/ShaderComponentMaskRender.h>
#include <shader/ShaderVariables.h>

ShaderComponentMaskRender::ShaderComponentMaskRender(Shader * _shader):
	ShaderComponent(_shader)
{
}

void ShaderComponentMaskRender::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	// Do nothing
}

void ShaderComponentMaskRender::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
}

std::string ShaderComponentMaskRender::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK_RENDER + ENDL;
}

std::string ShaderComponentMaskRender::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK_RENDER + ENDL;
}

std::string ShaderComponentMaskRender::getVertexBodyString(){
	return "";
}

std::string ShaderComponentMaskRender::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentMaskRender::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " = vec4(fragColor.a, fragColor.a, fragColor.a, 1.0)" + SEMI_ENDL;
}
