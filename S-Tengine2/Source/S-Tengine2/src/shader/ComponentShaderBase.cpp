#pragma once 

#include "shader/ComponentShaderBase.h"
#include "shader/ShaderVariables.h"
#include "node/NodeRenderable.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/GeometryComponent.h"

ComponentShaderBase::ComponentShaderBase(bool _autoRelease) :
	Shader(_autoRelease),
	geometryComponent(nullptr),
	lightingDirty(true)
{
}

ComponentShaderBase::ComponentShaderBase(std::vector<ShaderComponent*> _components, bool _autoRelease) :
	Shader(_autoRelease),
	components(_components),
	geometryComponent(nullptr)
{
}

std::string ComponentShaderBase::buildVertexShader(){

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
                                "in vec3 " + GL_ATTRIBUTE_ID_VERTEX_NORMALS + SEMI_ENDL +
                                "in vec4 " + GL_ATTRIBUTE_ID_VERTEX_COLOR + SEMI_ENDL +
                                "in vec2 " + GL_ATTRIBUTE_ID_VERTEX_UVS + SEMI_ENDL +
								
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
						TAB + "fragUV" + modGeo + " = aVertexUVs" + SEMI_ENDL;
						//TAB + "mat4 MVP = " + GL_UNIFORM_ID_MODEL_MATRIX +" * " + GL_UNIFORM_ID_VIEW_MATRIX + " * " + GL_UNIFORM_ID_PROJECTION_MATRIX + SEMI_ENDL +

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getVertexBodyString();
	}

	if(geometryComponent != nullptr){
		shaderString += geometryComponent->getVertexBodyString();
	}

	shaderString += "}";
	return shaderString;
}

std::string ComponentShaderBase::buildFragmentShader(){
	std::string shaderString  = "#version 150" + ENDL +

								"in vec3 fragVert" + SEMI_ENDL +
								"in vec3 fragNormal" + SEMI_ENDL +
								"in vec4 fragColor" + SEMI_ENDL +
								"in vec2 fragUV" + SEMI_ENDL +
								
								"out vec4 outColor" + SEMI_ENDL;

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

void ComponentShaderBase::compileShader(){
	//Shader::unload();
	if(geometryComponent != nullptr){
		init(buildVertexShader(), buildFragmentShader(), geometryComponent->getGeometryShader());	
	}else{
		init(buildVertexShader(), buildFragmentShader());	
	}
	isCompiled = true;
}

void ComponentShaderBase::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	for(unsigned long int i = 0; i < components.size(); ++i) {
		components.at(i)->configureUniforms(_matrixStack,_renderOption, _nodeRenderable);
	}
}

ComponentShaderBase::~ComponentShaderBase(){
	while(components.size() > 0){
		delete components.back();
		components.pop_back();
	}
}

void ComponentShaderBase::addComponent(ShaderComponent * _component){
	components.push_back(_component);
}

ShaderComponent* ComponentShaderBase::getComponentAt(int idx){
	return components.at(idx);
}

void ComponentShaderBase::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	Shader::clean(_matrixStack, _renderOption, _nodeRenderable);
	for(unsigned long int i = 0; i < components.size(); i++){
		components.at(i)->clean(_matrixStack, _renderOption, _nodeRenderable);
	}

	if(geometryComponent != nullptr){
		geometryComponent->clean(_matrixStack, _renderOption, _nodeRenderable);
	}
}

void ComponentShaderBase::unload(){
	if(loaded){
		for(ShaderComponent * sc : components){
			sc->unload();
		}
		Shader::unload();
		makeDirty();
		for(unsigned long int i = 0; i < components.size(); i++){
			components.at(i)->makeDirty();
		}
	}
}

void ComponentShaderBase::load(){
	if(!loaded){
		Shader::load();
		for(ShaderComponent * sc : components){
			sc->load();
		}
		compileShader();
	}
}