#pragma once

#include <shader/ShaderComponentDepth.h>
#include <shader/ShaderVariables.h>

ShaderComponentDepth::ShaderComponentDepth(ComponentShaderBase * _shader) :
	ShaderComponent(_shader)
{
}

void ShaderComponentDepth::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	// Do nothing
}

std::string ShaderComponentDepth::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_DEPTH + ENDL;
}

std::string ShaderComponentDepth::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_DEPTH + ENDL;
}

std::string ShaderComponentDepth::getVertexBodyString(){
	return "";
}


std::string ShaderComponentDepth::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentDepth::getOutColorMod(){
	return 
		"float zbuffer = gl_FragCoord.w;\n" + 
		GL_OUT_OUT_COLOR  + "*=" + "vec4(zbuffer, zbuffer, zbuffer, 1.0)" + SEMI_ENDL; 
}

void ShaderComponentDepth::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
}
