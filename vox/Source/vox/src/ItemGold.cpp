#pragma once

#include <ItemGold.h>
#include <Box2DWorld.h>
#include <Box2DSprite.h>
#include <RapunzelResourceManager.h>
#include <PuppetGame.h>
#include <MeshInterface.h>
#include <SpriteMesh.h>
#include <MeshFactory.h>

MeshInterface * ItemGold::goldMesh = nullptr;

ItemGold::ItemGold(Box2DWorld * _world) :
	ItemSimpleWeapon(RapunzelResourceManager::goldBrick, false, _world, PuppetGame::kITEM, PuppetGame::kGROUND | PuppetGame::kPLAYER),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	/*//if(goldMesh == nullptr){
		goldMesh = MeshFactory::getCubeMesh();//new SpriteMesh(GL_QUADS);
	//}
	delete rootComponent->mesh;
	rootComponent->mesh = goldMesh;*/

	/*if(goldMesh == nullptr){
		goldMesh = rootComponent->mesh;
	}else{
		//delete rootComponent->mesh;
		//rootComponent->mesh = goldMesh;
	}*/
}

void ItemGold::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption){
	ItemSimpleWeapon::render(_matrixStack, _renderOption);
}
