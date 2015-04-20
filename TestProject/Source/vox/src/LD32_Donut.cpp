#pragma once

#include <LD32_Donut.h>
#include <LD32_ResourceManager.h>
#include <LD32_Game.h>
#include <NumberUtils.h>
#include <Box2DWorld.h>
#include <Resource.h>
#include <MeshInterface.h>
#include <Material.h>
#include <Easing.h>

MeshInterface * LD32_Donut::donutTopMesh = nullptr;
MeshInterface * LD32_Donut::donutBotMesh = nullptr;
Material * LD32_Donut::donutMat = new Material(15.f, glm::vec3(1,1,1), true);


LD32_Donut::LD32_Donut(Box2DWorld * _world) :
	Box2DMeshEntity(_world, nullptr, b2_staticBody, false, nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	bot(nullptr),
	rotSpeed(vox::NumberUtils::randomFloat(-0.1f, 0.1f))
{
	b2Filter sf;
	sf.categoryBits = LD32_Game::kBUMPER;

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
	f->SetUserData(this);
	f->SetFilterData(sf);
	body->SetUserData(this);
	body->SetTransform(body->GetPosition(), vox::NumberUtils::randomFloat(0, 6.28));
}

void LD32_Donut::update(Step * _step){
	if(justHit){
		lastHit = _step->time;
		justHit = false;
	}

	float size = Easing::easeOutElastic(std::min(1.0, _step->time - lastHit), 2.5f, -1.5, 1);
	transform->scale(size, size, size, false);
	body->SetTransform(body->GetPosition(), body->GetAngle() + rotSpeed);

	Box2DMeshEntity::update(_step);
}


void LD32_Donut::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Box2DMeshEntity::render(_matrixStack, _renderOptions);
}

void LD32_Donut::setShader(Shader * _shader, bool _def){
	if(getShader() == nullptr){
		Box2DMeshEntity::setShader(_shader, _def);\
	}
}

LD32_Donut::~LD32_Donut(){
}

void LD32_Donut::hit(){
	justHit = true;
}