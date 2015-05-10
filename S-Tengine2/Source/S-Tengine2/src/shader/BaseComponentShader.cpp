#pragma once 

#include "shader/BaseComponentShader.h"
#include "shader/ShaderVariables.h"
#include "node/NodeRenderable.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/GeometryComponent.h"

BaseComponentShader::BaseComponentShader(bool _autoRelease) :
	Shader(_autoRelease),
	geometryComponent(nullptr),
	modelUniformLocation(-1),
	viewUniformLocation(-1),
	projectionUniformLocation(-1),
	mvpUniformLocation(-1)
{
}

BaseComponentShader::BaseComponentShader(std::vector<ShaderComponent*> _components, bool _autoRelease) :
	Shader(_autoRelease),
	components(_components),
	geometryComponent(nullptr)
{
}

std::string BaseComponentShader::buildVertexShader(){

	std::string modGeo = "";
	if(geometryComponent != nullptr){
		modGeo = GEO;
	}

	std::string shaderString  = "#version 150" + ENDL + 
							    /*"#extension GL_ARB_explicit_attrib_location : enable" + ENDL + 
								"layout(location = 1) in vec3 aVertexPosition" + SEMI_ENDL +
								"layout(location = 2) in vec4 aVertexColor" + SEMI_ENDL +
								"layout(location = 3) in vec3 aVertexNormals" + SEMI_ENDL +
								"layout(location = 4) in vec2 aVertexUVs" + SEMI_ENDL +*/

                                "in vec3 " + GL_ATTRIBUTE_ID_VERTEX_POSITION + SEMI_ENDL +
                                "in vec4 " + GL_ATTRIBUTE_ID_VERTEX_COLOR + SEMI_ENDL +
                                "in vec3 " + GL_ATTRIBUTE_ID_VERTEX_NORMALS + SEMI_ENDL +
                                "in vec2 " + GL_ATTRIBUTE_ID_VERTEX_UVS + SEMI_ENDL +
								
								"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_VIEW_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + SEMI_ENDL +
								
								"out vec3 fragVert" + modGeo + SEMI_ENDL +
								"out vec3 fragNormal" + modGeo + SEMI_ENDL +
								"out vec4 fragColor" + modGeo + SEMI_ENDL +
								"out vec2 fragUV" + modGeo + SEMI_ENDL;

	if(geometryComponent != nullptr){
		shaderString += geometryComponent->getVertexVariablesString();
	}

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getVertexVariablesString();
	}

	shaderString += "void main(){" + ENDL +
						TAB + "fragVert" + modGeo + "= aVertexPosition" + SEMI_ENDL +
						TAB + "fragNormal" + modGeo + " = aVertexNormals" + SEMI_ENDL +
						TAB + "fragColor" + modGeo + "= aVertexColor" + SEMI_ENDL +
						TAB + "fragUV" + modGeo + " = aVertexUVs" + SEMI_ENDL +
						//TAB + "mat4 MVP = " + GL_UNIFORM_ID_MODEL_MATRIX +" * " + GL_UNIFORM_ID_VIEW_MATRIX + " * " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +
						TAB + "gl_Position = " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + " * vec4(aVertexPosition, 1.0)" + SEMI_ENDL;

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getVertexBodyString();
	}

	if(geometryComponent != nullptr){
		shaderString += geometryComponent->getVertexBodyString();
	}

	shaderString += "}";
	return shaderString;
}

std::string BaseComponentShader::buildFragmentShader(){
	std::string shaderString  = "#version 150" + ENDL +

								"in vec3 fragVert" + SEMI_ENDL +
								"in vec3 fragNormal" + SEMI_ENDL +
								"in vec4 fragColor" + SEMI_ENDL +
								"in vec2 fragUV" + SEMI_ENDL +
								
								"out vec4 outColor" + SEMI_ENDL +
								
								"uniform mat4 " + GL_UNIFORM_ID_MODEL_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_VIEW_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +
								"uniform mat4 " + GL_UNIFORM_ID_MODEL_VIEW_PROJECTION + SEMI_ENDL;

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentVariablesString();
	}

	shaderString += "void main(){" + ENDL;

	shaderString += "vec4 modFrag = fragColor" + SEMI_ENDL;

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentBodyString();
	}

	shaderString += GL_OUT_OUT_COLOR + " = vec4(1)" + SEMI_ENDL;

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getOutColorMod();
	}

	shaderString += "}";
	return shaderString;
}

void BaseComponentShader::compileShader(){
	//Shader::unload();
	if(geometryComponent != nullptr){
		init(buildVertexShader(), buildFragmentShader(), geometryComponent->getGeometryShader());	
	}else{
		init(buildVertexShader(), buildFragmentShader());	
	}
	isCompiled = true;
}

void BaseComponentShader::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	const glm::mat4 * m = _matrixStack->getModelMatrix();
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
	
	m = _matrixStack->getViewMatrix();
	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
	
	m = _matrixStack->getProjectionMatrix();
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &(*m)[0][0]);

	m = _matrixStack->getMVP();
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &(*m)[0][0]);
	
	checkForGlError(0,__FILE__,__LINE__);
}

BaseComponentShader::~BaseComponentShader(){
	while(components.size() > 0){
		delete components.back();
		components.pop_back();
	}
}

void BaseComponentShader::addComponent(ShaderComponent * _component){
	components.push_back(_component);
}

ShaderComponent* BaseComponentShader::getComponentAt(int idx){
	return components.at(idx);
}

void BaseComponentShader::clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	Shader::clean(_matrixStack, _renderOption, _nodeRenderable);
	for(unsigned long int i = 0; i < components.size(); i++){
		components.at(i)->clean(_matrixStack, _renderOption, _nodeRenderable);
	}

	if(geometryComponent != nullptr){
		geometryComponent->clean(_matrixStack, _renderOption, _nodeRenderable);
	}
}

void BaseComponentShader::unload(){
	for(ShaderComponent * sc : components){
		sc->unload();
	}
	Shader::unload();
	makeDirty();
	for(unsigned long int i = 0; i < components.size(); i++){
		components.at(i)->makeDirty();
	}
}

void BaseComponentShader::load(){
	Shader::load();
	for(ShaderComponent * sc : components){
		sc->load();
	}
	compileShader();
	
	modelUniformLocation = glGetUniformLocation(getProgramId(), GL_UNIFORM_ID_MODEL_MATRIX.c_str());
	viewUniformLocation = glGetUniformLocation(getProgramId(), GL_UNIFORM_ID_VIEW_MATRIX.c_str());
	projectionUniformLocation = glGetUniformLocation(getProgramId(), GL_UNIFORM_ID_PROJECTION_MATRIX.c_str());
	mvpUniformLocation = glGetUniformLocation(getProgramId(), GL_UNIFORM_ID_MODEL_VIEW_PROJECTION.c_str());
}