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

void SharedComponentShaderMethods::configureLights(sweet::MatrixStack* _matrixStack, RenderOptions * _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	if(mesh != nullptr){
		// Pass the _shader the number of lights
		if(_renderOption->lights != nullptr){
			unsigned long int numLights = _renderOption->lights->size();
			glUniform1i(_renderOption->shader->numLightsUniformLocation, numLights);
			// Pass the paramaters for each light to the _shader
			for(unsigned long int i = 0; i < numLights; ++i){
				Light * l = _renderOption->lights->at(i);

				if(l->lightDirty){
					glm::vec3 curPos = l->lastPos;
					//std::cout << "update light " << i << ": " << curPos.x << "," << curPos.y << "," << curPos.z << std::endl;
					const std::string typ = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].type", i);
					const std::string pos = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_POSITION, i);
					const std::string ins = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_INTENSITIES, i);
					const std::string amb = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].ambientCoefficient", i);
					const std::string att = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].attenuation", i);
					const std::string cut = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].cutoff", i);
					const std::string ang = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].coneAngle", i);
					const std::string dir = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_LIGHTS_NO_ARRAY + "[].coneDirection", i);

					GLuint typeUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), typ.c_str());
					GLuint positionUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), pos.c_str());
					GLuint intensitiesUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), ins.c_str());
					GLuint ambientUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), amb.c_str());
					GLuint attenuationUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), att.c_str());
					GLuint cutoffUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), cut.c_str());
					GLuint angleUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), ang.c_str());
					GLuint coneDirectionUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), dir.c_str());

					glUniform1i(typeUniformLocation, static_cast<int>(l->getType()));
					glUniform3f(positionUniformLocation, curPos.x, curPos.y, curPos.z);
					glUniform3f(intensitiesUniformLocation, l->getIntensities().x, l->getIntensities().y, l->getIntensities().z);
					glUniform1f(ambientUniformLocation, l->getAmbientCoefficient());
					glUniform1f(attenuationUniformLocation, l->getAttenuation());
					glUniform1f(cutoffUniformLocation, l->getCutoff());
					glUniform3f(coneDirectionUniformLocation, l->getConeDirection().x, l->getConeDirection().y, l->getConeDirection().z);
					glUniform1f(angleUniformLocation, l->getConeAngle());
					l->lightDirty = false;
				}
			}
		}
	}
}

void SharedComponentShaderMethods::configureMaterials(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	MeshInterface * mesh = dynamic_cast<MeshInterface *>(_nodeRenderable);
	if(mesh != nullptr){
		unsigned long int numMaterials = mesh->materials.size();
		// Pass the _shader the number of materials
		glUniform1i(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_NUM_MATERIALS.c_str()), numMaterials);

		// Pass each material to the _shader
		for(unsigned long int i = 0; i < numMaterials; ++i){
			std::string shin = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_MATERIALS_NO_ARRAY+"[].shininess", i);
			std::string spec = GLUtils::buildGLArrayReferenceString(GL_UNIFORM_ID_MATERIALS_NO_ARRAY+"[].specularColor", i);
			GLuint shinyUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), shin.c_str());
			GLuint specColorUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), spec.c_str());
			glUniform1f(shinyUniformLocation, mesh->materials.at(i)->data.shininess);
			glUniform3f(specColorUniformLocation, mesh->materials.at(i)->data.specularColor.x, mesh->materials.at(i)->data.specularColor.y, mesh->materials.at(i)->data.specularColor.z);
		}
	}
}