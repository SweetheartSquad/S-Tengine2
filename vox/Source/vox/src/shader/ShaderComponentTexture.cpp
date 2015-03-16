#pragma 

#include "shader/ShaderComponentTexture.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "node/NodeRenderable.h"
#include "MeshInterface.h"
#include "Sprite.h"
#include "SpriteSheetAnimation.h"
#include "SpriteMesh.h"

ShaderComponentTexture::ShaderComponentTexture(Shader * _shader) :
	ShaderComponent(_shader){
}

ShaderComponentTexture::~ShaderComponentTexture(){
}

std::string ShaderComponentTexture::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_TEXTURE + ENDL;
}

std::string ShaderComponentTexture::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_TEXTURE + ENDL + 
		"uniform sampler2D " + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[" + std::to_string(MAX_LIGHTS) + "]" + SEMI_ENDL + 
		"uniform int " + GL_UNIFORM_ID_NUM_TEXTURES + SEMI_ENDL;
}

std::string ShaderComponentTexture::getVertexBodyString(){
	return "";
}

std::string ShaderComponentTexture::getFragmentBodyString(){
	return
		"if(" + GL_UNIFORM_ID_NUM_TEXTURES + " > 0){" + SEMI_ENDL +
			"for(int i = 0; i < " + GL_UNIFORM_ID_NUM_TEXTURES + "; i++){" + ENDL + 
			"	if(i == 0){" + ENDL + 
			"		modFrag = texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba" + SEMI_ENDL + 
			"	}else{" + ENDL + 
			"		modFrag = mix(modFrag, texture(" + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[i], " + GL_IN_OUT_FRAG_UV + ").rgba, 0.5)" + SEMI_ENDL + 
			"	}" + ENDL + 
			"}" + ENDL +
		"}" + ENDL;
}

std::string ShaderComponentTexture::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " *= modFrag" + SEMI_ENDL;
}

void ShaderComponentTexture::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	//ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
	configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	shader->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentTexture::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	int numTextures = 0;
	glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_TEXTURES.c_str()), 0);
	if(mesh != nullptr){
		glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_TEXTURES.c_str()), mesh->textures.size());
		// Bind each texture to the texture sampler array in the frag _shader
		for(unsigned long int i = 0; i < mesh->textures.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textures.at(i)->textureId);
			glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_TEXTURE_SAMPLER.c_str()), i);
		}
		numTextures = mesh->textures.size();
	}
	SpriteMesh * spriteMesh = dynamic_cast<SpriteMesh *>(_nodeRenderable);
	//Setup the texture for the current animation
	if(spriteMesh != nullptr){	
		if(spriteMesh->animatedTexture != nullptr){
			glActiveTexture(GL_TEXTURE0 + 1 + numTextures);
			glBindTexture(GL_TEXTURE_2D, spriteMesh->animatedTexture->textureId);
			glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_TEXTURE_SAMPLER.c_str()), numTextures + 1);
			glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_TEXTURES.c_str()), numTextures + 1);
		}
	}
}