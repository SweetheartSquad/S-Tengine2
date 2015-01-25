#pragma once

#include "Character.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "CharacterComponent.h"

Character::Character(Box2DWorld * _world) :
	MeshEntity(nullptr, new Transform()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.001f)
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

	torso		   ->transform->translate(0, 0, -0.01);
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

	for(NodeChild * s : children){
		dynamic_cast<Box2DSprite *>(s)->transform->scale(5,5,1);
	}

	torso->createFixture();
	head->createFixture();
	/*
	b2PolygonShape hShape;
	hShape.SetAsBox(1.6, 2.7);
	head->body->CreateFixture(&hShape, 1);
	*/
	//world->addToWorld(head);
	//world->addToWorld(torso);

	b2RevoluteJointDef jth;
	//jth.localAnchorB = b2Vec2(2.5/2 - 0.5, 1);
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	//jth.localAnchorA.Set(torso->width/2.f*torso->transform->scaleVector.x*componentScale, torso->height*torso->transform->scaleVector.y*componentScale);
	//jth.localAnchorB.Set(head->width/2.f*head->transform->scaleVector.x*componentScale, -head->height*head->transform->scaleVector.y*componentScale);
	jth.localAnchorA.Set(0, 0.8f * torso->height*torso->transform->scaleVector.y*componentScale*2.f);
	jth.localAnchorB.Set(0, 0.8f * -head->height*head->transform->scaleVector.y*componentScale*2.f);
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.referenceAngle = 0;
	jth.lowerAngle = -glm::radians(90.f);
	jth.upperAngle = glm::radians(90.f);
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