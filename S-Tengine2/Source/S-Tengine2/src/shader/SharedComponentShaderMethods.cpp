#pragma once

#include <shader/SharedComponentShaderMethods.h>
#include <RenderOptions.h>
#include <MatrixStack.h>
#include <node/NodeRenderable.h>
#include <GLUtils.h>
#include <MeshInterface.h>
#include <Material.h>
#include <Transform.h>
#include <SpotLight.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderVariables.h>

std::string SharedComponentShaderMethods::lightArrayAccessors[MAX_LIGHTS][8];
std::string SharedComponentShaderMethods::materialArrayAccessors[MAX_MATERIALS][2];
bool SharedComponentShaderMethods::staticInitialized = staticInit();

bool SharedComponentShaderMethods::staticInit(){
	for(unsigned long int i = 0; i < MAX_LIGHTS; ++i){
		std::stringstream ss;
		ss << GL_UNIFORM_ID_LIGHTS_NO_ARRAY << "[" << i << "]";
		std::string s = ss.str();
		lightArrayAccessors[i][0] = s + ".type";
		lightArrayAccessors[i][1] = s + ".position";
		lightArrayAccessors[i][2] = s + ".intensities";
		lightArrayAccessors[i][3] = s + ".ambientCoefficient";
		lightArrayAccessors[i][4] = s + ".attenuation";
		lightArrayAccessors[i][5] = s + ".cutoff";
		lightArrayAccessors[i][6] = s + ".coneAngle";
		lightArrayAccessors[i][7] = s + ".coneDirection";
	}
	for(unsigned long int i = 0; i < MAX_MATERIALS; ++i){
		std::stringstream ss;
		ss << GL_UNIFORM_ID_MATERIALS_NO_ARRAY << "[" << i << "]";
		std::string s = ss.str();
		materialArrayAccessors[i][0] = s + ".shininess";
		materialArrayAccessors[i][1] = s + ".specularColor";
	}
	return true;
}

void SharedComponentShaderMethods::configureLights(sweet::MatrixStack* _matrixStack, RenderOptions * _renderOption, NodeRenderable* _nodeRenderable){
	checkForGlError(false);
	ComponentShaderBase * shaderBase = dynamic_cast<ComponentShaderBase *>(_renderOption->shader);
	if(shaderBase->lightingDirty){
		// Pass the _shader the number of lights
		if(_renderOption->lights != nullptr){
			unsigned long int numLights = _renderOption->lights->size();
			glUniform1i(_renderOption->shader->numLightsUniformLocation, numLights);
			checkForGlError(false);
			// Pass the paramaters for each light to the _shader
			if(numLights > MAX_LIGHTS) {
				ST_LOG_WARN("Number of lights surpasses the max allowed number of lights")
			}
			for(unsigned long int i = 0; i < numLights && i <= MAX_LIGHTS; ++i){
				Light * l = _renderOption->lights->at(i);

				if(l->lightDirty){
					glm::vec3 curPos = l->lastPos;
					std::string * const root = lightArrayAccessors[i];

					GLuint typeUniformLocation =          glGetUniformLocation(_renderOption->shader->getProgramId(), root[0].c_str());
					GLuint positionUniformLocation =      glGetUniformLocation(_renderOption->shader->getProgramId(), root[1].c_str());
					GLuint intensitiesUniformLocation =   glGetUniformLocation(_renderOption->shader->getProgramId(), root[2].c_str());
					GLuint ambientUniformLocation =       glGetUniformLocation(_renderOption->shader->getProgramId(), root[3].c_str());
					GLuint attenuationUniformLocation =   glGetUniformLocation(_renderOption->shader->getProgramId(), root[4].c_str());
					GLuint cutoffUniformLocation =        glGetUniformLocation(_renderOption->shader->getProgramId(), root[5].c_str());
					GLuint angleUniformLocation =         glGetUniformLocation(_renderOption->shader->getProgramId(), root[6].c_str());
					GLuint coneDirectionUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), root[7].c_str());

					glUniform1i(typeUniformLocation, static_cast<int>(l->getType()));
					glUniform3f(positionUniformLocation, curPos.x, curPos.y, curPos.z);
					glm::vec3 intensities = l->getIntensities();
					glUniform3f(intensitiesUniformLocation, intensities.x, intensities.y, intensities.z);
					glUniform1f(ambientUniformLocation, l->getAmbientCoefficient());
					glUniform1f(attenuationUniformLocation, l->getAttenuation());
					glUniform1f(cutoffUniformLocation, l->getCutoff());
					glm::vec3 coneDirection = l->getConeDirection();
					glUniform3f(coneDirectionUniformLocation, coneDirection.x, coneDirection.y, coneDirection.z);
					glUniform1f(angleUniformLocation, l->getConeAngle());
					checkForGlError(false);
				}
			}
		}
	}
	shaderBase->lightingDirty = false;
	checkForGlError(false);
}

void SharedComponentShaderMethods::configureMaterials(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	if(mesh != nullptr){
		unsigned long int numMaterials = mesh->materials.size();
		// Pass the _shader the number of materials
		glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_MATERIALS.c_str()), numMaterials);

		// Pass each material to the _shader
		for(unsigned long int i = 0; i < numMaterials; ++i){
			std::string * const root = materialArrayAccessors[i];
			GLuint shinyUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), root[0].c_str());
			GLuint specColorUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), root[1].c_str());
			glUniform1f(shinyUniformLocation, mesh->materials.at(i)->data.shininess);
			glUniform3f(specColorUniformLocation, mesh->materials.at(i)->data.specularColor.x, mesh->materials.at(i)->data.specularColor.y, mesh->materials.at(i)->data.specularColor.z);
		}
	}
}