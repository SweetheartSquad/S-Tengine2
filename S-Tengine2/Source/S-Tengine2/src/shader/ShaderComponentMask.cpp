#pragma once

#include <shader/ShaderComponentMask.h>
#include <shader/ShaderVariables.h>
#include <shader/Shader.h>
#include <shader/ShaderComponentDepth.h>
#include <MeshInterface.h>

ShaderComponentMask::ShaderComponentMask(Shader * _shader) :
	ShaderComponent(_shader),
	maskTextureId(0)
{
}

void ShaderComponentMask::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	int offset = 0;
	MeshInterface * mi = dynamic_cast<MeshInterface *>(_nodeRenderable);
	
	if(mi != nullptr) {
		offset = mi->textures.size();
	}
	
	GLint texSamLoc = glGetUniformLocation(shader->getProgramId(), "maskTextureSampler");
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, maskTextureId);
	glUniform1i(texSamLoc, offset);
}

std::string ShaderComponentMask::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_MASK + ENDL;
}

std::string ShaderComponentMask::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_MASK + ENDL +
		"uniform sampler2D maskTextureSampler" + SEMI_ENDL;
}

std::string ShaderComponentMask::getVertexBodyString(){
	return "";
}


std::string ShaderComponentMask::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentMask::getOutColorMod(){
	return 
		"vec4 maskTexColor = texture(maskTextureSampler, " + GL_IN_OUT_FRAG_UV + ")" + SEMI_ENDL + 
		GL_OUT_OUT_COLOR  + ".a *= maskTexColor.r" + SEMI_ENDL;
}

void ShaderComponentMask::setMaskTextureId(GLuint _id){
	maskTextureId = _id;
	makeDirty();
}
