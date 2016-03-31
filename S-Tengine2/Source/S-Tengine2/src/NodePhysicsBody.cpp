#pragma once

#include <NodePhysicsBody.h>

NodePhysicsBody::NodePhysicsBody() :
	directAdjustment(true)
{
}

NodePhysicsBody::~NodePhysicsBody(){
}

void NodePhysicsBody::translatePhysical(glm::vec3 _translation, bool _relative){
	directAdjustment = true;
}

void NodePhysicsBody::realign(){
	directAdjustment = false;
}

void NodePhysicsBody::update(Step * _step){
	// purposefully left empty to avoid duplicate calls to Entity::update in derived classes
}

void NodePhysicsBody::applyForceToCenter(glm::vec3 _force){
	applyForce(_force, getPhysicsBodyCenter());
}

void NodePhysicsBody::applyForceLeft(float _force){
	applyForceToCenter(glm::vec3(-_force, 0, 0));
}

void NodePhysicsBody::applyForceRight(float _force){
	applyForceToCenter(glm::vec3(_force, 0, 0));
}

void NodePhysicsBody::applyForceUp(float _force){
	applyForceToCenter(glm::vec3(0, _force, 0));
}

void NodePhysicsBody::applyForceDown(float _force){
	applyForceToCenter(glm::vec3(0, -_force, 0));
}

void NodePhysicsBody::applyLinearImpulseToCenter(glm::vec3 _force){
	applyLinearImpulse(_force, getPhysicsBodyCenter());
}

void NodePhysicsBody::applyLinearImpulseLeft(float _force){
	applyLinearImpulseToCenter(glm::vec3(-_force, 0, 0));
}

void NodePhysicsBody::applyLinearImpulseRight(float _force){
	applyLinearImpulseToCenter(glm::vec3(_force, 0, 0));
}

void NodePhysicsBody::applyLinearImpulseUp(float _force){
	applyLinearImpulseToCenter(glm::vec3(0, _force, 0));
}

void NodePhysicsBody::applyLinearImpulseDown(float _force){
	applyLinearImpulseToCenter(glm::vec3(0, -_force, 0));
}