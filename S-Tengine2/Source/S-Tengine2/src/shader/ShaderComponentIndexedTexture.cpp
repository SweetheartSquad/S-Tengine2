#pragma 

#include "shader/ShaderComponentIndexedTexture.h"
#include "shader/ShaderVariables.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "node/NodeRenderable.h"
#include "MeshInterface.h"
#include "Sprite.h"
#include "SpriteSheetAnimation.h"

ShaderComponentIndexedTexture::ShaderComponentIndexedTexture(Shader * _shader) :
	ShaderComponent(_shader){
}

ShaderComponentIndexedTexture::~ShaderComponentIndexedTexture(){
}

std::string ShaderComponentIndexedTexture::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_TEXTURE + ENDL;
}

std::string ShaderComponentIndexedTexture::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_TEXTURE + ENDL + 
		"uniform sampler2D " + GL_UNIFORM_ID_TEXTURE_SAMPLER + "[" + std::to_string(MAX_TEXTURES) + "]" + SEMI_ENDL + 
		"uniform int " + GL_UNIFORM_ID_NUM_TEXTURES + SEMI_ENDL;
}

std::string ShaderComponentIndexedTexture::getVertexBodyString(){
	return "";
}

std::string ShaderComponentIndexedTexture::getFragmentBodyString(){
    std::stringstream res;
    res << "if(" << GL_UNIFORM_ID_NUM_TEXTURES << " == 2){" << ENDL;
		// access the first texture to get the indexed colour
		res << "vec4 myIndex = texture2D(" << GL_UNIFORM_ID_TEXTURE_SAMPLER +"[0], " << GL_IN_OUT_FRAG_UV << ");";
		// access the second texture AT the colour index
		res << "vec4 texel = texture2D(" << GL_UNIFORM_ID_TEXTURE_SAMPLER +"[1], myIndex.xy);";
		// output the texture
		res << "modFrag = texel;";
		//res << "modFrag = vec4(myIndex.xy,0,1);";
    res << "}" << ENDL;
    return res.str();
}

std::string ShaderComponentIndexedTexture::getOutColorMod(){
	return GL_OUT_OUT_COLOR + " *= modFrag" + SEMI_ENDL;
}

void ShaderComponentIndexedTexture::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
	//configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	//shader->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentIndexedTexture::load(){
	if(!loaded){
		texNumLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_NUM_TEXTURES.c_str());
		texColLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_TEXT_COLOR.c_str());
		texSamLoc = glGetUniformLocation(shader->getProgramId(), GL_UNIFORM_ID_TEXTURE_SAMPLER.c_str());
	}
	ShaderComponent::load();
}

void ShaderComponentIndexedTexture::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	int numTextures = 0;
	if(mesh != nullptr){
		numTextures = mesh->textureCount();
		glUniform1i(texNumLoc, numTextures);
		// Bind each texture to the texture sampler array in the frag _shader
		for(unsigned long int i = 0; i < mesh->textureCount(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->getTexture(i)->textureId);
			glUniform1i(texSamLoc, i);
		}
	}else{
		glUniform1i(texNumLoc, numTextures);
	}

	/*SpriteMesh * spriteMesh = dynamic_cast<SpriteMesh *>(_nodeRenderable);
	//Setup the texture for the current animation
	if(spriteMesh != nullptr){	
		if(spriteMesh->animatedTexture != nullptr){
			glActiveTexture(GL_TEXTURE0 + 1 + numTextures);
			glBindTexture(GL_TEXTURE_2D, spriteMesh->animatedTexture->textureId);
			glUniform1i(texSamLoc, numTextures + 1);
			glUniform1i(texNumLoc, numTextures + 1);
		}
	}*/
}
