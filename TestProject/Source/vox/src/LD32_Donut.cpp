#pragma once

#include <LD32_Donut.h>
#include <LD32_ResourceManager.h>
#include <NumberUtils.h>
#include <Box2DWorld.h>
#include <Resource.h>
#include <MeshInterface.h>
#include <Material.h>

MeshInterface * LD32_Donut::donutTopMesh = nullptr;
MeshInterface * LD32_Donut::donutBotMesh = nullptr;
Material * LD32_Donut::donutMat = new Material(15.f, glm::vec3(1,1,1), true);
	
LD32_Donut::LD32_Donut(Box2DWorld * _world) :
	Box2DMeshEntity(_world, nullptr, b2_dynamicBody, false, nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform())
{
	if(donutTopMesh == nullptr){
		donutTopMesh = Resource::loadMeshFromObj("../assets/donutTop.obj");
		donutTopMesh->pushMaterial(donutMat);
		donutTopMesh->pushTexture2D(LD32_ResourceManager::donutTop);
		donutTopMesh->dirty = true;
	}if(donutBotMesh == nullptr){
		donutBotMesh = Resource::loadMeshFromObj("../assets/donutBot.obj");
		donutBotMesh->pushMaterial(donutMat);
		donutBotMesh->pushTexture2D(LD32_ResourceManager::donutBot);
		donutBotMesh->dirty = true;
	}
	mesh = donutTopMesh;
	donutTopMesh->referenceCount++;
	bot = new MeshEntity(donutBotMesh);
	addChild(bot);

	world->addToWorld(this);
	createFixture();

	body->SetLinearDamping(5.f);
	body->SetAngularDamping(5.f);
}
