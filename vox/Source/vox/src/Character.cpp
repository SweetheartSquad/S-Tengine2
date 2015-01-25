#pragma once

#include "Character.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "CharacterComponent.h"
#include "Scene.h"

Character::Character(Box2DWorld * _world) :
	MeshEntity(nullptr, new Transform()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.0025f)
{
}

Character::~Character(){
}

void Character::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void Character::update(Step * _step){
	MeshEntity::update(_step);
	b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
	float angle = neck->GetJointAngle();

	neck->SetMotorSpeed(-angle*360);
	neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));

	float bodAngle = torso->body->GetAngle();
	torso->body->SetAngularVelocity(-bodAngle*2);
	
	/*if(angle < neck->GetLowerLimit()/2 || angle > neck->GetUpperLimit()/2){
		neck->SetMotorSpeed(angle < 0 ? 0.1 : -0.1);
		neck->SetMaxMotorTorque(head->body->GetMass()*1000);
	}else{
		neck->SetMotorSpeed(0);
		neck->SetMaxMotorTorque(0);
	}*/
}

void Character::attachJoints(){
	

	/*for(NodeChild * s : children){
		dynamic_cast<Box2DSprite *>(s)->transform->scale(5,5,1);
	}*/

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
	jth.enableMotor = true;
	jth.maxMotorTorque = 0;//head->body->GetMass()*1000;
	jth.motorSpeed = 0;//
	jth.referenceAngle = 0;
	jth.lowerAngle = -glm::radians(45.f);
	jth.upperAngle = glm::radians(45.f);
	world->b2world->CreateJoint(&jth);

	torso		   ->setTranslationPhysical(0, 0, 1);
	head		   ->setTranslationPhysical(0, 0, -1);
	leftUpperArm   ->setTranslationPhysical(0, 0, -0.01);
	leftLowerArm   ->setTranslationPhysical(0, 0, -0.02);
	leftHand	   ->setTranslationPhysical(0, 0, -0.03);
	rightUpperArm  ->setTranslationPhysical(0, 0, 0.01);
	rightLowerArm  ->setTranslationPhysical(0, 0, 0.02);
	rightHand	   ->setTranslationPhysical(0, 0, 0.03);
	upperLeftLeg   ->setTranslationPhysical(0, 0, -0.01);
	lowerLeftLeg   ->setTranslationPhysical(0, 0, -0.02);
	upperRightLeg  ->setTranslationPhysical(0, 0, 0.01);
	lowerRightLeg  ->setTranslationPhysical(0, 0, 0.02);
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

void Character::addToScene(Scene * _scene){
	_scene->addChild(torso);
	_scene->addChild(leftUpperArm);
	_scene->addChild(leftLowerArm);
	_scene->addChild(leftHand);
	_scene->addChild(rightUpperArm);
	_scene->addChild(rightLowerArm);
	_scene->addChild(rightHand);
	_scene->addChild(upperLeftLeg);
	_scene->addChild(lowerLeftLeg);
	_scene->addChild(upperRightLeg);
	_scene->addChild(lowerRightLeg);
	_scene->addChild(head);
}