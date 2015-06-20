#include <shader/ShaderComponentNormals.h>

#include <MatrixStack.h>
#include <shader/ShaderVariables.h>
#include <RenderOptions.h>

#include <GL/glew.h>

#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.inl>

ShaderComponentNormals::ShaderComponentNormals(Shader * _shader) :
	ShaderComponent(_shader)
{
}

void ShaderComponentNormals::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	// Do nothing
}

std::string ShaderComponentNormals::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_NORMAL + ENDL;
}

std::string ShaderComponentNormals::getFragmentVariablesString(){
	return DEFINE + SHADER_COMPONENT_NORMAL + ENDL;
}

std::string ShaderComponentNormals::getVertexBodyString(){
	return "";
}


std::string ShaderComponentNormals::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentNormals::getOutColorMod(){
	return GL_OUT_OUT_COLOR  + "*=" + "vec4(normalize(fragNormal.rgb), 1.0)" + SEMI_ENDL; 
}

void ShaderComponentNormals::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	makeDirty();
	ShaderComponent::clean(_matrixStack, _renderOption, _nodeRenderable);
}
