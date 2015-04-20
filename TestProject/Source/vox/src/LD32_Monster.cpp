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
	NodeTransformable(new Transform())
{
	b2Filter sf;
	sf.categoryBits = LD32_Game::kMONSTER;
	mesh->pushMaterial(new Material(15.f, glm::vec3(1,1,1), true));

	world->addToWorld(this);
	b2Fixture * f = createFixture(true);
	f->SetRestitution(2.5f);
	f->SetFilterData(sf);
	f->SetUserData(this);
	body->SetUserData(this);
}

void LD32_Monster::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Box2DMeshEntity::render(_matrixStack, _renderOptions);
}

void LD32_Monster::setShader(Shader * _shader, bool _def){
	if(getShader() == nullptr){
		Box2DMeshEntity::setShader(_shader, _def);
	}
}

LD32_Monster::~LD32_Monster(){
}