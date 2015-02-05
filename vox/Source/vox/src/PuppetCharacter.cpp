#pragma once

#include "PuppetCharacter.h"
#include <CharacterComponent.h>
#include <Texture.h>

PuppetCharacter::PuppetCharacter(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, bool _ai):
	Character(_world, _categoryBits, _maskBits, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	ComponentTexture * headTex = new ComponentTexture(new Texture("../assets/character components/MichaelHead.png", 512, 512, true, true), 160, 270);
	CharacterComponent * head = new CharacterComponent(1.f, headTex->width, headTex->height, headTex->texture, _world);
	components.push_back(head);
}

PuppetCharacter::~PuppetCharacter(){
}

void PuppetCharacter::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void PuppetCharacter::update(Step* _step){
	MeshEntity::update(_step);
}

void PuppetCharacter::unload(){
	MeshEntity::unload();
}

void PuppetCharacter::load(){
	MeshEntity::load();
}