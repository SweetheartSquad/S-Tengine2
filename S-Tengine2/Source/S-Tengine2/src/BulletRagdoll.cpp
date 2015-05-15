#pragma once

#include <BulletRagdoll.h>

#include <BulletMeshEntity.h>
#include <BulletWorld.h>
#include <MeshFactory.h>
#include <MeshInterface.h>

BulletRagdoll::BulletRagdoll(BulletWorld * _world){
	body = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(body);
	body->parents.at(0)->scale(3, 5, 1);
	body->setColliderAsBox(3, 5, 1);
	body->createRigidBody(1, 2, 1);

	head = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(head);
	head->parents.at(0)->scale(2, 2, 2);
	head->setColliderAsBox(2, 2, 2);
	head->createRigidBody(1, 2, 1);

	upperlegLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperlegLeft);
	upperlegLeft->parents.at(0)->scale(1, 3, 1);
	upperlegLeft->setColliderAsBox(1, 3, 1);
	upperlegLeft->createRigidBody(1, 2, 1);

	upperlegRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperlegRight);
	upperlegRight->parents.at(0)->scale(1, 3, 1);
	upperlegRight->setColliderAsBox(1, 3, 1);
	upperlegRight->createRigidBody(1, 2, 1);

	lowerlegLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerlegLeft);
	lowerlegLeft->parents.at(0)->scale(1, 3, 1);
	lowerlegLeft->setColliderAsBox(1, 3, 1);
	lowerlegLeft->createRigidBody(1, 2, 1);

	lowerlegRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerlegRight);
	lowerlegRight->parents.at(0)->scale(1, 3, 1);
	lowerlegRight->setColliderAsBox(1, 3, 1);
	lowerlegRight->createRigidBody(1, 2, 1);

	upperarmLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperarmLeft);
	upperarmLeft->parents.at(0)->scale(1, 3, 1);
	upperarmLeft->setColliderAsBox(1, 3, 1);
	upperarmLeft->createRigidBody(1, 2, 1);

	upperarmRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(upperarmRight);
	upperarmRight->parents.at(0)->scale(1, 3, 1);
	upperarmRight->setColliderAsBox(1, 3, 1);
	upperarmRight->createRigidBody(1, 2, 1);

	lowerarmLeft = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerarmLeft);
	lowerarmLeft->parents.at(0)->scale(1, 3, 1);
	lowerarmLeft->setColliderAsBox(1, 3, 1);
	lowerarmLeft->createRigidBody(1, 2, 1);

	lowerarmRight = new BulletMeshEntity(_world, MeshFactory::getCubeMesh(1));
	childTransform->addChild(lowerarmRight);
	lowerarmRight->parents.at(0)->scale(1, 3, 1);
	lowerarmRight->setColliderAsBox(1, 3, 1);
	lowerarmRight->createRigidBody(1, 2, 1);
	
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

	// neck
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, 0, 0);
	frame2.getBasis().setEulerZYX(0, 0, 0);
	frame1.setOrigin(btVector3(0,6,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*body->body, *head->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, halfpi);
	_world->world->addConstraint(coneConstraint, true);
	
	// left hip
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, halfpi, 0);
	frame2.getBasis().setEulerZYX(0, halfpi, 0);
	frame1.setOrigin(btVector3(2,-5,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*body->body, *upperlegLeft->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, 0);
	//coneConstraint->setLimit(0.001, 0.001, 0);
	_world->world->addConstraint(coneConstraint, true);

	// right hip
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, halfpi, 0);
	frame2.getBasis().setEulerZYX(0, halfpi, 0);
	frame1.setOrigin(btVector3(-2,-5,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*body->body, *upperlegRight->body, frame1, frame2);
	coneConstraint->setLimit(quarterpi, quarterpi, 0);
	//coneConstraint->setLimit(0.001, 0.001, 0);
	_world->world->addConstraint(coneConstraint, true);

	// left knee
	hingeConstraint = new btHingeConstraint(*upperlegLeft->body, *lowerlegLeft->body, btVector3(0,3,0), btVector3(0,-3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, halfpi);
	_world->world->addConstraint(hingeConstraint, true);
	
	// right knee
	hingeConstraint = new btHingeConstraint(*upperlegRight->body, *lowerlegRight->body, btVector3(0,3,0), btVector3(0,-3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, halfpi);
	_world->world->addConstraint(hingeConstraint, true);
	
	// left shoulder
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, halfpi, 0);
	frame2.getBasis().setEulerZYX(0, halfpi, 0);
	frame1.setOrigin(btVector3(4,5,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*body->body, *upperarmLeft->body, frame1, frame2);
	coneConstraint->setLimit(halfpi, halfpi, 0);
	//coneConstraint->setLimit(0.001, 0.001, 0);
	_world->world->addConstraint(coneConstraint, true);

	// right shoulder
	frame1.setIdentity(); frame2.setIdentity();
	frame1.getBasis().setEulerZYX(0, halfpi, 0);
	frame2.getBasis().setEulerZYX(0, halfpi, 0);
	frame1.setOrigin(btVector3(-4,5,0));
	frame2.setOrigin(btVector3(0,-3,0));
	coneConstraint = new btConeTwistConstraint(*body->body, *upperarmRight->body, frame1, frame2);
	coneConstraint->setLimit(halfpi, halfpi, 0);
	//coneConstraint->setLimit(0.001, 0.001, 0);
	_world->world->addConstraint(coneConstraint, true);
	
	
	// left elbow
	hingeConstraint = new btHingeConstraint(*upperarmLeft->body, *lowerarmLeft->body, btVector3(0,3,0), btVector3(0,-3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, halfpi);
	_world->world->addConstraint(hingeConstraint, true);

	// right elbow
	hingeConstraint = new btHingeConstraint(*upperarmRight->body, *lowerarmRight->body, btVector3(0,3,0), btVector3(0,-3,0), btVector3(1, 0, 0), btVector3(1, 0, 0), false); 
	hingeConstraint->setLimit(0, halfpi);
	_world->world->addConstraint(hingeConstraint, true);

	//body->body->setcoll
}

void BulletRagdoll::setShader(Shader * _shader, bool _default){
	body->setShader(_shader, _default);
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