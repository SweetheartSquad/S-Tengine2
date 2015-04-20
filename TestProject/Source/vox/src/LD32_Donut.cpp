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
	Box2DMeshEntity(_world, nullptr, b2_staticBody, false, nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	bot(nullptr)
{
	//if(donutTopMesh == nullptr){
		donutTopMesh = Resource::loadMeshFromObj("../assets/donutTop.vox");
		donutTopMesh->pushMaterial(donutMat);
		donutTopMesh->pushTexture2D(LD32_ResourceManager::donutTop);
	//}if(donutBotMesh == nullptr){
		donutBotMesh = Resource::loadMeshFromObj("../assets/donutBot.vox");
		donutBotMesh->pushMaterial(donutMat);
		donutBotMesh->pushTexture2D(LD32_ResourceManager::donutBot);
	//}

	bot = new MeshEntity(nullptr);
	bot->mesh = donutBotMesh;
	++donutBotMesh->referenceCount;
	addChild(bot);

	mesh = donutTopMesh;
	++donutTopMesh->referenceCount;

	world->addToWorld(this);
	b2Fixture * f = createFixture(true);
	f->SetRestitution(2.5f);
}

void LD32_Donut::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Box2DMeshEntity::render(_matrixStack, _renderOptions);
}

void LD32_Donut::setShader(Shader * _shader, bool _def){
	if(getShader() == nullptr){
		Box2DMeshEntity::setShader(_shader, _def);
		//bot->setShader(_shader, _def);
	}
}

LD32_Donut::~LD32_Donut(){
}