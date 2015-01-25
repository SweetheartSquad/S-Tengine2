#pragma once

#include "Character.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"

Character::Character(Box2DWorld * _world) :
	MeshEntity(nullptr, new Transform()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world)
{
}

Character::~Character(){
}

void Character::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void Character::update(Step* _step){
	MeshEntity::update(_step);
}

void Character::attachJoints(){
	addChild(torso);
	addChild(leftUpperArm);
	addChild(leftLowerArm);
	addChild(leftHand);
	addChild(rightUpperArm);
	addChild(rightLowerArm);
	addChild(rightHand);
	addChild(upperLeftLeg);
	addChild(lowerLeftLeg);
	addChild(upperRightLeg);
	addChild(lowerRightLeg);
	addChild(head);

	torso		   ->transform->translate(0, 0, 0);
	head		   ->transform->translate(0, 0, 0.01);
	leftUpperArm   ->transform->translate(0, 0, -0.01);
	leftLowerArm   ->transform->translate(0, 0, -0.02);
	leftHand	   ->transform->translate(0, 0, -0.03);
	rightUpperArm  ->transform->translate(0, 0, 0.01);
	rightLowerArm  ->transform->translate(0, 0, 0.02);
	rightHand	   ->transform->translate(0, 0, 0.03);
	upperLeftLeg   ->transform->translate(0, 0, -0.01);
	lowerLeftLeg   ->transform->translate(0, 0, -0.02);
	upperRightLeg  ->transform->translate(0, 0, 0.01);
	lowerRightLeg  ->transform->translate(0, 0, 0.02);

	b2PolygonShape tShape;
	tShape.SetAsBox(2.5, 3);
	torso->body->CreateFixture(&tShape, 1);

	b2PolygonShape hShape;
	hShape.SetAsBox(1, 1);
	head->body->CreateFixture(&hShape, 1);

	//world->addToWorld(head);
	//world->addToWorld(torso);

	b2RevoluteJointDef jth;
	jth.localAnchorB = b2Vec2(2.5/2 - 0.5, 1);
	jth.bodyA = head->body;
	jth.bodyB = torso->body;
	world->b2world->CreateJoint(&jth);
}

void Character::unload(){
}

void Character::load(){
}

void Character::setShader(Shader * _shader){
	torso	      ->setShader(_shader, true);
	head		  ->setShader(_shader, true);
	leftUpperArm  ->setShader(_shader, true);
	leftLowerArm  ->setShader(_shader, true);
	leftHand	  ->setShader(_shader, true);
	rightUpperArm ->setShader(_shader, true);
	rightLowerArm ->setShader(_shader, true);
	rightHand	  ->setShader(_shader, true);
	upperLeftLeg  ->setShader(_shader, true);
	lowerLeftLeg  ->setShader(_shader, true);
	upperRightLeg ->setShader(_shader, true);
	lowerRightLeg ->setShader(_shader, true);
}