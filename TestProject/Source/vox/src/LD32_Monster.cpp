#pragma once

#include <LD32_Monster.h>
#include <LD32_Game.h>
#include <LD32_ResourceManager.h>
#include <NumberUtils.h>
#include <Box2DWorld.h>
#include <Resource.h>
#include <MeshInterface.h>
#include <Material.h>
	
LD32_Monster::LD32_Monster(Box2DWorld * _world) :
	Box2DMeshEntity(_world, Resource::loadMeshFromObj("../assets/monster1.vox"), b2_staticBody, false, nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	spawnEnemy(0)
{
	b2Filter sf;
	sf.categoryBits = LD32_Game::kMONSTER;
	sf.maskBits = LD32_Game::kPLAYER;
	mesh->pushMaterial(new Material(15.f, glm::vec3(1,1,1), true));

	world->addToWorld(this);
	b2Fixture * f = createFixture(true);
	f->SetRestitution(2.5f);
	f->SetFilterData(sf);
	f->SetUserData(this);
	body->SetUserData(this);
}

void LD32_Monster::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	glm::quat t = transform->getOrientationQuat();
	transform->rotate(-90, 1, 0, 0, kWORLD);
	transform->rotate(90, 0, 0, 1, kWORLD);
	Box2DMeshEntity::render(_matrixStack, _renderOptions);
	transform->setOrientation(t);
}

void LD32_Monster::setShader(Shader * _shader, bool _def){
	if(getShader() == nullptr){
		Box2DMeshEntity::setShader(_shader, _def);
	}
}

LD32_Monster::~LD32_Monster(){
}