#pragma once 

#include "shader/BaseComponentShader.h"
#include "shader/ShaderVariables.h"
#include "node/NodeRenderable.h"
#include "MatrixStack.h"
#include "RenderOptions.h"
#include "shader/GeometryComponent.h"

BaseComponentShader::BaseComponentShader() :
	Shader(false),
	geometryComponent(nullptr){
}

BaseComponentShader::BaseComponentShader(std::vector<ShaderComponent*> _components) :
	Shader(false),
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
							    "#extension GL_ARB_explicit_attrib_location : enable" + ENDL + 
								"layout(location = 1) in vec3 aVertexPosition" + SEMI_ENDL +
								"layout(location = 2) in vec4 aVertexColor" + SEMI_ENDL +
								"layout(location = 3) in vec3 aVertexNormals" + SEMI_ENDL +
								"layout(location = 4) in vec2 aVertexUVs" + SEMI_ENDL +

								"uniform mat4 MVP" + SEMI_ENDL +
								
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

	shaderString += "void main(){\n"
						"fragVert" + modGeo + "= aVertexPosition;\n"
						"fragNormal" + modGeo + " = aVertexNormals;\n"
						"fragColor" + modGeo + "= aVertexColor;\n"
						"fragUV" + modGeo + " = aVertexUVs;\n"
						"gl_Position = MVP * vec4(aVertexPosition, 1.0);\n";

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
	std::string shaderString  = "#version 150\n"

								"in vec3 fragVert;\n"
								"in vec3 fragNormal;\n"
								"in vec4 fragColor;\n"
								"in vec2 fragUV;\n"
								
								"out vec4 outColor;\n"
								
								"uniform mat4 model;\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentVariablesString();
	}

	shaderString += "void main(){\n";

	shaderString += "vec4 modFrag = fragColor;\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentBodyString();
	}

	shaderString += "outColor = vec4(1, 1, 1, 1);\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getOutColorMod();
	}

	shaderString += "}";
	return shaderString;
}

void BaseComponentShader::compileShader(){
	Shader::unload();
	if(geometryComponent != nullptr){
		init(buildVertexShader(), buildFragmentShader(), geometryComponent->getGeometryShader());	
	}else{
		init(buildVertexShader(), buildFragmentShader());	
	}
	
}

void BaseComponentShader::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){

	glm::mat4 mvp = _matrixStack->projectionMatrix * _matrixStack->viewMatrix * _matrixStack->currentModelMatrix;
	GLuint mvpUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(),  GL_UNIFORM_ID_MODEL_VIEW_PROJECTION.c_str());
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glm::mat4 model = _matrixStack->currentModelMatrix;
	GLuint modelUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_MODEL_MATRIX.c_str());
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);
	GLUtils::checkForError(0,__FILE__,__LINE__);
	
	for(unsigned long int i = 0; i < components.size(); i++){
		components.at(i)->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	}

	if(geometryComponent != nullptr){
		geometryComponent->configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
	}
}

BaseComponentShader::~BaseComponentShader(){
	for(unsigned long int i = 0; i < components.size(); i++){
		delete components.at(i);
	}
}

