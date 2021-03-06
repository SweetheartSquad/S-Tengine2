#pragma once

#include <BulletMeshEntity.h>
#include <MeshInterface.h>
#include <algorithm>

#include <algorithm>

BulletMeshEntity::BulletMeshEntity(BulletWorld * _world, MeshInterface * _mesh, Shader * _shader) :
	MeshEntity(_mesh, _shader),
	NodeBulletBody(_world)
{
}

void BulletMeshEntity::update(Step* _step){
	NodeBulletBody::update(_step);
	MeshEntity::update(_step);
}

void BulletMeshEntity::setColliderAsBoundingBox(){
	sweet::Box bb = mesh->calcBoundingBox();
	setColliderAsBox(bb.width*0.5f, bb.height*0.5f, bb.depth*0.5f);
	
	// this moves the mesh so that it is on top of the collider, and then sets the collider position to be where the mesh was
	meshTransform->translate(-bb.x - bb.width*0.5f, -bb.y - bb.height*0.5f, -bb.z - bb.depth*0.5f);
	internalPos.setValue(bb.x + bb.width*0.5f, bb.y + bb.height*0.5f, bb.z + bb.depth*0.5f);
}

void BulletMeshEntity::setColliderAsBoundingSphere(bool _encompassBox){
	sweet::Box bb = mesh->calcBoundingBox();
	float diameter = _encompassBox ? glm::length(glm::vec3(bb.width, bb.height, bb.depth)) : std::max(bb.width, std::max(bb.height, bb.depth));
	setColliderAsSphere(diameter*0.5f);
	
	// this moves the mesh so that it is on top of the collider, and then sets the collider position to be where the mesh was
	meshTransform->translate(-bb.x - bb.width*0.5f, -bb.y - bb.height*0.5f, -bb.z - bb.depth*0.5f);
	internalPos.setValue(bb.x + bb.width*0.5f, bb.y + bb.height*0.5f, bb.z + bb.depth*0.5f);
}