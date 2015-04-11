#pragma once

#include <ScoreIndicator.h>
#include <PuppetResourceManager.h>
#include <MeshInterface.h>
#include <TextureSampler.h>
#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentHsv.h>
#include <RenderOptions.h>
#include <ParticleSystem.h>

ScoreIndicator::ScoreIndicator(unsigned long int _id, Box2DWorld * _world) : 
	Sprite(nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	id(_id),
	scoreParticles(new ParticleSystem(PuppetResourceManager::scoreParticles.at(0), _world, 0, 0, 0))
{
	float pixels = 100;
	transform->scale(pixels, pixels, 1);
	transform->translate((3.5f-id) * 1920.f*0.25f, pixels*1.1f, 10.f);
	//scoreIndicator->transform->rotate(90, 0, 1, 0, kOBJECT);
	mesh->pushTexture2D(PuppetResourceManager::scoreIndicators.at(id)->texture);
}

void ScoreIndicator::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	ShaderComponentHsv * hsvShader = static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(getShader())->components.at(1));
	float hue = hsvShader->getHue();
	float sat = hsvShader->getSaturation();
	float newHue = hue, newSat = sat;
	if(id == 0){
		newSat = sat + 0.8f;
		newHue = 0.125f;
	}else if(id == 1){
		newHue = 0.3056f;
	}else if(id == 2){
		newHue = 0.64f;
		newSat = sat +0.55f;
	}else if(id == 3){
		newHue = 0.f;
	}

	// change the shader settings based player id
	hsvShader->setHue(newHue);
	hsvShader->setSaturation(newSat);

	Sprite::render(_matrixStack, _renderOptions);
	scoreParticles->render(_matrixStack, _renderOptions);
	
	// revert the shader settings
	hsvShader->setHue(hue);
	hsvShader->setSaturation(sat);
}

void ScoreIndicator::update(Step * _step){
	Sprite::update(_step);
	scoreParticles->update(_step);
}
void ScoreIndicator::setShader(Shader * _shader, bool _configure){
	Sprite::setShader(_shader, _configure);
	scoreParticles->setShader(_shader, _configure);
}