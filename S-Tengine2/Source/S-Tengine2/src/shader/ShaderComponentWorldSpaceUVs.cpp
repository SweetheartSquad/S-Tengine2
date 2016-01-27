#pragma 

#include "shader/ShaderComponentWorldSpaceUVs.h"
#include "shader/ShaderVariables.h"
#include "shader/ComponentShaderBase.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "node/NodeRenderable.h"
#include "MeshInterface.h"
#include "Sprite.h"
#include "SpriteSheetAnimation.h"

ShaderComponentWorldSpaceUVs::ShaderComponentWorldSpaceUVs(ComponentShaderBase * _shader) :
	ShaderComponent(_shader),
	xMultiplier(1),
	yMultiplier(1),
	xMultiplierLoc(-1),
	yMultiplierLoc(-1)
{
}

ShaderComponentWorldSpaceUVs::~ShaderComponentWorldSpaceUVs(){
}

std::string ShaderComponentWorldSpaceUVs::getVertexVariablesString(){
	return 
		IF_DEFINED + SHADER_COMPONENT_TEXTURE + ENDL + 
		DEFINE + SHADER_COMPONENT_WORLDSPACEUVS + ENDL +
		"uniform float xMultiplier, yMultiplier" + SEMI_ENDL +
		END_IF + ENDL;
}

std::string ShaderComponentWorldSpaceUVs::getFragmentVariablesString(){
	return 
		IF_DEFINED + SHADER_COMPONENT_TEXTURE + ENDL + 
		DEFINE + SHADER_COMPONENT_WORLDSPACEUVS + ENDL +
		END_IF + ENDL;
}

std::string ShaderComponentWorldSpaceUVs::getVertexBodyString(){
	return GL_IN_OUT_FRAG_UV + " = (" + GL_UNIFORM_ID_MODEL_MATRIX + " * vec4(aVertexPosition, 1.0)" + ").xy * vec2(-xMultiplier, -yMultiplier)" + SEMI_ENDL;
}

std::string ShaderComponentWorldSpaceUVs::getFragmentBodyString(){
    return "";
}

std::string ShaderComponentWorldSpaceUVs::getOutColorMod(){
	return "";
}

void ShaderComponentWorldSpaceUVs::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
	//configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	//shader->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
}

void ShaderComponentWorldSpaceUVs::load(){
	if(!loaded){
		xMultiplierLoc = glGetUniformLocation(shader->getProgramId(), "xMultiplier");
		yMultiplierLoc = glGetUniformLocation(shader->getProgramId(), "yMultiplier");
	}
	ShaderComponent::load();
}

void ShaderComponentWorldSpaceUVs::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	glUniform1f(xMultiplierLoc, xMultiplier);
	glUniform1f(yMultiplierLoc, yMultiplier);
}
