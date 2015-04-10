#pragma once

#include <PuppetCharacterGuard.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourManager.h>
#include <RapunzelResourceManager.h>
#include <shader\ShaderComponentTint.h>
#include <shader\ShaderComponentAlpha.h>
#include <RenderOptions.h>
#include <shader\BaseComponentShader.h>

#include <Item.h>

PuppetCharacterGuard::PuppetCharacterGuard(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Torso : RapunzelResourceManager::guard2Torso,
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Arm : RapunzelResourceManager::guard2Arm,
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Helmet : RapunzelResourceManager::guard2Helmet
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	//itemHolder = armLeft;
	behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(0, 0, 0), glm::vec3(70.f, 0, 0), this, 10));
	behaviourManager->addBehaviour(new BehaviourAttack(this, 3, PuppetGame::kPLAYER));
}

PuppetCharacterGuard::~PuppetCharacterGuard(){
}


void PuppetCharacterGuard::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// save the current shader settings
	float red = static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->getRed();
	float green = static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->getGreen();
	float blue = static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->getBlue();
	float alpha = static_cast<ShaderComponentAlpha *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(3))->getAlpha();

	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setRed(red + (1 - control) * 3);
	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setGreen(green - (1 - control) * 3);
	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setBlue(blue - (1 - control) * 3);

	if (dead){
		static_cast<ShaderComponentAlpha *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(3))->setAlpha(0.5f);
	}


	armLeft->render(_matrixStack, _renderOptions);
	armRight->render(_matrixStack, _renderOptions);
	torso->render(_matrixStack, _renderOptions);
	head->render(_matrixStack, _renderOptions);
	face->render(_matrixStack, _renderOptions);
	handLeft->render(_matrixStack, _renderOptions);
	handRight->render(_matrixStack, _renderOptions);
	headgear->render(_matrixStack, _renderOptions);

	if (indicator != nullptr){
		indicator->render(_matrixStack, _renderOptions);
	}
	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setRed(red);
	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setGreen(green);
	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(2))->setBlue(blue);
	static_cast<ShaderComponentAlpha *>(static_cast<BaseComponentShader *>(_renderOptions->shader)->components.at(3))->setAlpha(alpha);
}