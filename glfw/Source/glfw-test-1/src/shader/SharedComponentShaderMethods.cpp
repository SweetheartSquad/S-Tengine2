#pragma once

#include "shader/SharedComponentShaderMethods.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "node/NodeRenderable.h"
#include "GLUtils.h"
#include "MeshInterface.h"
#include "Material.h"
#include "Transform.h"

void SharedComponentShaderMethods::configureLights(vox::MatrixStack* _matrixStack, RenderOptions * _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	if(mesh != nullptr){
		// Pass the _shader the number of lights
		glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_LIGHTS.c_str()), _renderOption->lights->size());

		// Pass the paramaters for each light to the _shader
		for(unsigned long int i = 0; i < _renderOption->lights->size(); i++){
			std::string typ = GLUtils::buildGLArrayReferenceString("lights[].type", i);
			std::string pos = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_POSITION, i);
			std::string ins = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_INTENSITIES, i);
			std::string amb = GLUtils::buildGLArrayReferenceString("lights[].ambientCoefficient", i);
			std::string att = GLUtils::buildGLArrayReferenceString("lights[].attenuation", i);
			GLuint typeUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), typ.c_str());
			glUniform1i(typeUniformLocation, static_cast<int>(_renderOption->lights->at(i)->data.type));
			GLuint positionUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), pos.c_str());
			glUniform3f(positionUniformLocation, _renderOption->lights->at(i)->transform->translationVector.x, _renderOption->lights->at(i)->transform->translationVector.y, _renderOption->lights->at(i)->transform->translationVector.z);
			GLuint intensitiesUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), ins.c_str());
			glUniform3f(intensitiesUniformLocation, _renderOption->lights->at(i)->data.intensities.x, _renderOption->lights->at(i)->data.intensities.y, _renderOption->lights->at(i)->data.intensities.z);
			GLuint ambientUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), amb.c_str());
			glUniform1f(ambientUniformLocation, _renderOption->lights->at(i)->data.ambientCoefficient);
			GLuint attenuationUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), att.c_str());
			glUniform1f(attenuationUniformLocation, _renderOption->lights->at(i)->data.attenuation);
		}
	}
}

void SharedComponentShaderMethods::configureMaterials(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	if(mesh != nullptr){
		// Pass the _shader the number of materials
		glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), "numMaterials"), mesh->materials.size());

		// Pass each material to the _shader
		for(unsigned long int i = 0; i < mesh->materials.size(); i++){
			std::string shin = GLUtils::buildGLArrayReferenceString("materials[].shininess", i);
			std::string spec = GLUtils::buildGLArrayReferenceString("materials[].specularColor", i);
			GLuint shinyUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), shin.c_str());

			int materialShininess = mesh->materials.at(i)->data.shininess;
			glUniform1f(shinyUniformLocation, materialShininess);
			GLuint specColorUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), spec.c_str());
			glUniform3f(specColorUniformLocation, mesh->materials.at(i)->data.specularColor.x, mesh->materials.at(i)->data.specularColor.y, mesh->materials.at(i)->data.specularColor.z);
		}
	}
}