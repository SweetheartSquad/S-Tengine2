#pragma once

#include <BulletMeshEntity.h>

BulletMeshEntity::BulletMeshEntity(BulletWorld * _world, MeshInterface * _mesh) :
	MeshEntity(_mesh),
	NodeBulletBody(_world, false)
{

}

BulletMeshEntity::~BulletMeshEntity(){
}

void BulletMeshEntity::update(Step* _step){
	NodeBulletBody::update(_step);
	MeshEntity::update(_step);
}