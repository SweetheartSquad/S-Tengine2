#pragma once

#include <NodePhysicsBody.h>

NodePhysicsBody::NodePhysicsBody() :	
	directAdjustment(true)
{
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