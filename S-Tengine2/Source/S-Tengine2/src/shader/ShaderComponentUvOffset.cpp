#pragma 

#include "shader/ShaderComponentUvOffset.h"
#include "shader/ShaderVariables.h"
#include "shader/ComponentShaderBase.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "node/NodeRenderable.h"
#include "MeshInterface.h"
#include "Sprite.h"
#include "SpriteSheetAnimation.h"

ShaderComponentUvOffset::ShaderComponentUvOffset(ComponentShaderBase * _shader) :
	ShaderComponent(_shader),
	xOffset(0),
	yOffset(0),
	xOffsetLoc(-1),
	yOffsetLoc(-1)
{
}

ShaderComponentUvOffset::~ShaderComponentUvOffset(){
}

std::string ShaderComponentUvOffset::getVertexVariablesString(){
	return 
		IF_DEFINED + SHADER_COMPONENT_TEXTURE + ENDL + 
		DEFINE + SHADER_COMPONENT_UV_OFFSET + ENDL +
		"uniform float xOffset, yOffset" + SEMI_ENDL +
		END_IF + ENDL;
}

std::string ShaderComponentUvOffset::getFragmentVariablesString(){
	return 
		IF_DEFINED + SHADER_COMPONENT_TEXTURE + ENDL + 
		DEFINE + SHADER_COMPONENT_UV_OFFSET + ENDL +
		END_IF + ENDL;
}

std::string ShaderComponentUvOffset::getVertexBodyString(){
	return GL_IN_OUT_FRAG_UV + " += vec2(-xOffset, -yOffset)" + SEMI_ENDL;
}

std::string ShaderComponentUvOffset::getFragmentBodyString(){
    return "";
}

std::string ShaderComponentUvOffset::getOutColorMod(){
	return "";
}

void ShaderComponentUvOffset::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
	//configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	//shader->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentUvOffset::load(){
	if(!loaded){
		xOffsetLoc = glGetUniformLocation(shader->getProgramId(), "xOffset");
		yOffsetLoc = glGetUniformLocation(shader->getProgramId(), "yOffset");
	}
	ShaderComponent::load();
}

void ShaderComponentUvOffset::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	glUniform1f(xOffsetLoc, xOffset);
	glUniform1f(yOffsetLoc, yOffset);
}
