#pragma once

#include <LD32_Enemy.h>
#include <LD32_Game.h>

#include <MeshInterface.h>
#include <Resource.h>
#include <Box2DWorld.h>

LD32_Enemy::LD32_Enemy(Box2DWorld * _world) :
	Box2DMeshEntity(_world, nullptr, b2_dynamicBody, false, nullptr, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	heDed(false)
{
	b2Filter sf;
	sf.categoryBits = LD32_Game::kENEMY;

	if(std::rand() % 2 == 0){
		mesh = Resource::loadMeshFromObj("../assets/Mushroom.vox");
	}else{
		mesh = Resource::loadMeshFromObj("../assets/ThingGuy.vox");
	}

	world->addToWorld(this);
	b2Fixture * f = createFixture(true);
	f->SetRestitution(1.f);
	f->SetUserData(this);
	f->SetFilterData(sf);
	body->SetUserData(this);
}