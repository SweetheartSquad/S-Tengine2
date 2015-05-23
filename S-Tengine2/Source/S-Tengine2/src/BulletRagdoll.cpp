#pragma once

#include <BulletRagdoll.h>

#include <BulletMeshEntity.h>
#include <BulletWorld.h>
#include <MeshFactory.h>
#include <MeshInterface.h>

BulletRagdoll::BulletRagdoll(BulletWorld * _world){
	upperbody = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperbody);
	upperbody->parents.at(0)->scale(3, 3, 1);
	upperbody->setColliderAsBox(3, 3, 1);
	upperbody->createRigidBody(1);

	lowerbody = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerbody);
	lowerbody->parents.at(0)->scale(3, 2, 1);
	lowerbody->setColliderAsBox(3, 2, 1);
	lowerbody->createRigidBody(1);

	head = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(head);
	head->parents.at(0)->scale(2, 2, 2);
	head->setColliderAsBox(2, 2, 2);
	head->createRigidBody(1);

	upperlegLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperlegLeft);
	upperlegLeft->parents.at(0)->scale(1, 3, 1);
	upperlegLeft->setColliderAsBox(1, 3, 1);
	upperlegLeft->createRigidBody(1);

	upperlegRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperlegRight);
	upperlegRight->parents.at(0)->scale(1, 3, 1);
	upperlegRight->setColliderAsBox(1, 3, 1);
	upperlegRight->createRigidBody(1);

	lowerlegLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerlegLeft);
	lowerlegLeft->parents.at(0)->scale(1, 3, 1);
	lowerlegLeft->setColliderAsBox(1, 3, 1);
	lowerlegLeft->createRigidBody(1);

	lowerlegRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerlegRight);
	lowerlegRight->parents.at(0)->scale(1, 3, 1);
	lowerlegRight->setColliderAsBox(1, 3, 1);
	lowerlegRight->createRigidBody(1);

	upperarmLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperarmLeft);
	upperarmLeft->parents.at(0)->scale(1, 3, 1);
	upperarmLeft->setColliderAsBox(1, 3, 1);
	upperarmLeft->createRigidBody(1);

	upperarmRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperarmRight);
	upperarmRight->parents.at(0)->scale(1, 3, 1);
	upperarmRight->setColliderAsBox(1, 3, 1);
	upperarmRight->createRigidBody(1);

	lowerarmLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerarmLeft);
	lowerarmLeft->parents.at(0)->scale(1, 3, 1);
	lowerarmLeft->setColliderAsBox(1, 3, 1);
	lowerarmLeft->createRigidBody(1);

	lowerarmRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerarmRight);
	lowerarmRight->parents.at(0)->scale(1, 3, 1);
	lowerarmRight->setColliderAsBox(1, 3, 1);
	lowerarmRight->createRigidBody(1);
	
	// colour code sides for debugging purposes
	for(unsigned long int i = 0; i < upperlegLeft->mesh->vertices.size(); ++i){
		upperlegLeft->mesh->vertices.at(i).red = 0;
		lowerlegLeft->mesh->vertices.at(i).red = 0;
		upperarmLeft->mesh->vertices.at(i).red = 0;
		lowerarmLeft->mesh->vertices.at(i).red = 0;
		upperlegLeft->mesh->vertices.at(i).green = 0;
		lowerlegLeft->mesh->vertices.at(i).green = 0;
		upperarmLeft->mesh->vertices.at(i).green = 0;
		lowerarmLeft->mesh->vertices.at(i).green = 0;
		
		upperlegRight->mesh->vertices.at(i).blue = 0;
		lowerlegRight->mesh->vertices.at(i).blue = 0;
		upperarmRight->mesh->vertices.at(i).blue = 0;
		lowerarmRight->mesh->vertices.at(i).blue = 0;
		upperlegRight->mesh->vertices.at(i).green = 0;
		lowerlegRight->mesh->vertices.at(i).green = 0;
		upperarmRight->mesh->vertices.at(i).green = 0;
		lowerarmRight->mesh->vertices.at(i).green = 0;
		
		head->mesh->vertices.at(i).red = 0;
		upperbody->mesh->vertices.at(i).red = 0;
		lowerbody->mesh->vertices.at(i).red = 0;
		head->mesh->vertices.at(i).blue = 0;
		upperbody->mesh->vertices.at(i).blue = 0;
		lowerbody->mesh->vertices.at(i).blue = 0;
	}
	
	btPoint2PointConstraint * p2pconstraint;
	btHingeConstraint * hingeConstraint;
	btConeTwistConstraint * coneConstraint;
	btTransform frame1;
	btTransform frame2;
	
	
	float pi = 3.14159f;
	float pi2 = pi*2.f;
	float halfpi = pi*0.5f;
	float quarterpi = halfpi*0.5f;
	
	// spine
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, halfpi);
	frame2.getBasis().setEulerZYX(0, 0, halfpi);
	frame1.setOrigin(btVector3(0,-3,0));
	frame2.setOrigin(btVector3(0,2,0));
	coneConstraint = new btConeTwistConstraint(*upperbody->body, *lowerbody->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, halfpi);
	//coneConstraint->setMotorTargetInConstraintSpace(btQuaternion(0, 1, 0, 1));
	_world->world->addConstraint(coneConstraint, true);

	// neck
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, halfpi);
	frame2.getBasis().setEulerZYX(0, 0, halfpi);
	frame1.setOrigin(btVector3(0,4,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*upperbody->body, *head->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, halfpi);
	//coneConstraint->setMotorTargetInConstraintSpace(btQuaternion(0, 1, 0, 1));
	_world->world->addConstraint(coneConstraint, true);
	
	// left hip
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, 0);
	frame2.getBasis().setEulerZYX(0, 0, 0);
	frame1.setOrigin(btVector3(2,-2,0));
	frame2.setOrigin(btVector3(0,3,0));
	coneConstraint = new btConeTwistConstraint(*lowerbody->body, *upperlegLeft->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, 0);
	//coneConstraint->setLimit(0, 0, 0);
	_world->world->addConstraint(coneConstraint, true);

	// right hip
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, 0);
	frame2.getBasis().setEulerZYX(0, 0, 0);
	frame1.setOrigin(btVector3(-2,-2,0));
	frame2.setOrigin(btVector3(0,3,0));
	coneConstraint = new btConeTwistConstraint(*lowerbody->body, *upperlegRight->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, 0);
	//coneConstraint->setLimit(0, 0, 0);
	_world->world->addConstraint(coneConstraint, true);

	// left knee
	hingeConstraint = new btHingeConstraint(*upperlegLeft->body, *lowerlegLeft->body, btVector3(0,-3,0), btVector3(0,3,0), btVector3(-1, 0, 0), btVector3(-1, 0, 0), false); 
	hingeConstraint->setLimit(0, pi*0.8f);
	//hingeConstraint->enableAngularMotor(true, 10, 10);
	_world->world->addConstraint(hingeConstraint, true);
	
	// right knee
	hingeConstraint = new btHingeConstraint(*upperlegRight->body, *lowerlegRight->body, btVector3(0,-3,0), btVector3(0,3,0), btVector3(-1, 0, 0), btVector3(-1, 0, 0), false); 
	hingeConstraint->setLimit(0, pi*0.8f);
	//hingeConstraint->enableAngularMotor(true, 10, 10);
	_world->world->addConstraint(hingeConstraint, true);
	
	// left shoulder
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, pi);
	frame2.getBasis().setEulerZYX(halfpi, 0, halfpi);
	frame1.setOrigin(btVector3(4,3,0));
	frame2.setOrigin(btVector3(0,3,0));
	coneConstraint = new btConeTwistConstraint(*upperbody->body, *upperarmLeft->body, frame1, frame2);
	coneConstraint->setLimit(halfpi, pi*0.75f, 0);
	//btGeneric6DofConstraint * dofConstraint = new btGeneric6DofConstraint(*body->body, *upperarmLeft->body, frame1, frame2, true);
	//coneConstraint->setLimit(0, 0, 0);
	_world->world->addConstraint(coneConstraint, true);

	// right shoulder
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, pi);
	frame2.getBasis().setEulerZYX(halfpi, 0, -halfpi);
	frame1.setOrigin(btVector3(-4,3,0));
	frame2.setOrigin(btVector3(0,3,0));
	coneConstraint = new btConeTwistConstraint(*upperbody->body, *upperarmRight->body, frame1, frame2);
	coneConstraint->setLimit(halfpi, pi*0.75f, 0);
	//coneConstraint->setLimit(0.001, 0.001, 0);
	_world->world->addConstraint(coneConstraint, true);
	
	
	// left elbow
	hingeConstraint = new btHingeConstraint(*upperarmLeft->body, *lowerarmLeft->body, btVector3(0,-3,0), btVector3(0,3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, pi*0.8f);
	_world->world->addConstraint(hingeConstraint, true);

	// right elbow
	hingeConstraint = new btHingeConstraint(*upperarmRight->body, *lowerarmRight->body, btVector3(0,-3,0), btVector3(0,3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, pi*0.8f);
	_world->world->addConstraint(hingeConstraint, true);
}

void BulletRagdoll::setShader(Shader * _shader, bool _default){
	upperbody->setShader(_shader, _default);
	lowerbody->setShader(_shader, _default);
	head->setShader(_shader, _default);
	upperlegLeft->setShader(_shader, _default);
	upperlegRight->setShader(_shader, _default);
	lowerlegLeft->setShader(_shader, _default);
	lowerlegRight->setShader(_shader, _default);
	upperarmLeft->setShader(_shader, _default);
	upperarmRight->setShader(_shader, _default);
	lowerarmLeft->setShader(_shader, _default);
	lowerarmRight->setShader(_shader, _default);
}