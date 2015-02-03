#pragma once

#include "PuppetCharacter.h"

PuppetCharacter::PuppetCharacter(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, bool _ai):
	MeshEntity(nullptr, new Transform()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.0025f),
	groupIndex(--gGroupIndex),
	categoryBits(_categoryBits),
	maskBits(_maskBits),
	ai(_ai)
{
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