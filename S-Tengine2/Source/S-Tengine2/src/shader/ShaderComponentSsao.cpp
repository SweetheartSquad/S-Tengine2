#pragma once

#include <shader/ShaderComponentSsao.h>
#include <shader/ShaderVariables.h>
#include <MeshInterface.h>
#include <RenderOptions.h>
#include <Log.h>

ShaderComponentSsao::ShaderComponentSsao(Shader * _shader) :
	ShaderComponent(_shader)
{
}

void ShaderComponentSsao::configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	if( _renderOption->normalBufferId > 0 && _renderOption->depthBufferId > 0){
		MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
		int numTextures = 0;
		if(mesh != nullptr){
			numTextures = mesh->textureCount();
		}
		// Bind the normal buffer texture
		glActiveTexture(GL_TEXTURE0 + numTextures);
		glBindTexture(GL_TEXTURE_2D,  _renderOption->normalBufferId);
		
		// Bind the depth buffer texture
		glActiveTexture(GL_TEXTURE0 + numTextures + 1);
		glBindTexture(GL_TEXTURE_2D,  _renderOption->normalBufferId);	
	}else{
		Log::warn("_renderOptions must contain a valid depth buffer texture and a valid normal buffer texture in order to use the SSAO shader component");
	}
}

std::string ShaderComponentSsao::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_SSAO + ENDL;
}

std::string ShaderComponentSsao::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_SSAO + ENDL;
}

std::string ShaderComponentSsao::getVertexBodyString(){
	return "";
}


std::string ShaderComponentSsao::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentSsao::getOutColorMod(){
	return 
		"float zbuffer = gl_FragCoord.w;\n" + 
		GL_OUT_OUT_COLOR  + "*=" + "vec4(zbuffer, zbuffer, zbuffer, 1.0)" + SEMI_ENDL; 
}

void ShaderComponentSsao::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
}
