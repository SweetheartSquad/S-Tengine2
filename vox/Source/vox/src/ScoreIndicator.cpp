#pragma once

#include <ScoreIndicator.h>
#include <PuppetResourceManager.h>
#include <MeshInterface.h>
#include <TextureSampler.h>
#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentHsv.h>
#include <RenderOptions.h>

ScoreIndicator::ScoreIndicator(unsigned long int _id) : 
	Sprite(nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	id(_id)
{
	transform->scale(100, 100, 1);
	transform->translate((1.5f-id)*1920.f*0.25f, -1080.f*0.4f, 5);
	//scoreIndicator->transform->rotate(90, 0, 1, 0, kOBJECT);
	mesh->pushTexture2D(PuppetResourceManager::scoreIndicators.at(id)->texture);
}

void ScoreIndicator::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	float hue = static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->getHue();
	float sat = static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->getSaturation();
	float newHue = hue, newSat = sat;
	if(id == 0){
		newSat = sat + 0.8f;
		newHue = 0.125f;
	}else if(id == 1){
		newHue = 0.3056f;
	}else if(id == 2){
		newHue = 0.64;
		newSat = sat +0.55f;
	}else if(id == 3){
		newHue = 0;
	}

	// change the shader settings based player id
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->setHue(newHue);
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->setSaturation(newSat);

	Sprite::render(_matrixStack, _renderOptions);
	
	// revert the shader settings
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->setHue(hue);
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(1))->setSaturation(sat);
}