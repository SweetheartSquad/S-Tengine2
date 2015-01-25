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
	ratioX_neck_to_torso = 0.0f;
	ratioY_neck_to_torso = 0.8f;
	ratioX_torso_to_neck = 0.0f;
	ratioY_torso_to_neck = 0.8f;
	
	ratioX_shoulder_to_torso = 0.f;
	ratioY_shoulder_to_torso = 1.f;
	ratioX_torso_to_shoulder = 0.8f;
	ratioY_torso_to_shoulder = 0.5f;
	
	ratioX_hip_to_torso = 0.f;
	ratioY_hip_to_torso = 1.f;
	ratioX_torso_to_hip = 0.55f;
	ratioY_torso_to_hip = 0.7f;
	
	ratioX_knee_to_hip = 0.5f;
	ratioY_knee_to_hip = 0.8f;
	ratioX_hip_to_knee = 0.75f;
	ratioY_hip_to_knee = 0.8f;
	
	ratioX_elbow_to_shoulder;
	ratioY_elbow_to_shoulder;
	ratioX_shoulder_to_elbow;
	ratioY_shoulder_to_elbow;
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
	torso->body->SetAngularVelocity(-bodAngle*5);
	
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

	
	float correctedTorsoWidth = torso->getCorrectedWidth();
	float correctedTorsoHeight = torso->getCorrectedHeight();

	// neck
	b2RevoluteJointDef jth;
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	jth.localAnchorA.Set(ratioX_torso_to_neck * correctedTorsoWidth, ratioY_torso_to_neck * correctedTorsoHeight);
	jth.localAnchorB.Set(ratioX_neck_to_torso * head->getCorrectedWidth(), ratioY_neck_to_torso * -head->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.enableMotor = true;
	jth.maxMotorTorque = 0;
	jth.motorSpeed = 0;
	jth.referenceAngle = 0;
	jth.lowerAngle = -glm::radians(45.f);
	jth.upperAngle = glm::radians(45.f);
	world->b2world->CreateJoint(&jth);

	// left shoulder
	leftUpperArm->createFixture();
	b2RevoluteJointDef latj;
	latj.bodyA = torso->body;
	latj.bodyB = leftUpperArm->body;
	latj.localAnchorA.Set(-ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	latj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	latj.collideConnected = false;
	latj.referenceAngle = glm::radians(-90.f);
	world->b2world->CreateJoint(&latj);
	

	// right shoulder
	rightUpperArm->createFixture();
	b2RevoluteJointDef ratj;
	ratj.bodyA = torso->body;
	ratj.bodyB = rightUpperArm->body;
	ratj.localAnchorA.Set(ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	ratj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	ratj.collideConnected = false;
	ratj.referenceAngle = glm::radians(90.f);
	world->b2world->CreateJoint(&ratj);

	// left elbow
	/*rightUpperArm->createFixture();
	b2RevoluteJointDef letj;
	ratj.bodyA = torso->body;
	ratj.bodyB = rightUpperArm->body;
	ratj.localAnchorA.Set(ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	ratj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	ratj.collideConnected = false;
	ratj.referenceAngle = glm::radians(90.f);
	world->b2world->CreateJoint(&ratj);*/


	// left hip
	upperLeftLeg->createFixture();
	b2RevoluteJointDef lltj;
	lltj.bodyA = torso->body;
	lltj.bodyB = upperLeftLeg->body;
	lltj.localAnchorA.Set(-ratioX_torso_to_hip * correctedTorsoWidth, -ratioY_torso_to_hip * correctedTorsoHeight);
	lltj.localAnchorB.Set(ratioX_hip_to_torso * upperLeftLeg->getCorrectedWidth(),  ratioY_hip_to_torso * upperLeftLeg->getCorrectedHeight());
	lltj.collideConnected = false;
	world->b2world->CreateJoint(&lltj);
	
	// right hip
	upperRightLeg->createFixture();
	b2RevoluteJointDef lrtj;
	lrtj.bodyA = torso->body;
	lrtj.bodyB = upperRightLeg->body;
	lrtj.localAnchorA.Set(ratioX_torso_to_hip * correctedTorsoWidth, -ratioY_torso_to_hip * correctedTorsoHeight);
	lrtj.localAnchorB.Set(ratioX_hip_to_torso * upperLeftLeg->getCorrectedWidth(), ratioY_hip_to_torso * upperRightLeg->getCorrectedHeight());
	lrtj.collideConnected = false;
	world->b2world->CreateJoint(&lrtj);

	// left knee
	lowerLeftLeg->createFixture();
	b2RevoluteJointDef llltj;
	llltj.bodyA = upperLeftLeg->body;
	llltj.bodyB = lowerLeftLeg->body;
	llltj.localAnchorA.Set(-ratioX_hip_to_knee * upperLeftLeg->getCorrectedWidth(), -ratioY_hip_to_knee * upperLeftLeg->getCorrectedHeight());
	llltj.localAnchorB.Set(ratioX_knee_to_hip * lowerLeftLeg->getCorrectedWidth(), ratioY_knee_to_hip * lowerLeftLeg->getCorrectedHeight());
	llltj.collideConnected = false;
	world->b2world->CreateJoint(&llltj);
	
	// right knee
	lowerRightLeg->createFixture();
	b2RevoluteJointDef lrltj;
	lrltj.bodyA = upperRightLeg->body;
	lrltj.bodyB = lowerRightLeg->body;
	lrltj.localAnchorA.Set(-ratioX_hip_to_knee * upperRightLeg->getCorrectedWidth(), -ratioY_hip_to_knee * upperRightLeg->getCorrectedHeight());
	lrltj.localAnchorB.Set(-ratioX_knee_to_hip * lowerRightLeg->getCorrectedWidth(), ratioY_knee_to_hip * lowerRightLeg->getCorrectedHeight());
	lrltj.collideConnected = false;
	world->b2world->CreateJoint(&lrltj);

	
	torso		   ->setTranslationPhysical(0.f, 0.f, 1.f);
	head		   ->setTranslationPhysical(0.f, 0.f, -1.f);
	leftUpperArm   ->setTranslationPhysical(0.f, 0.f, -0.01f);
	leftLowerArm   ->setTranslationPhysical(0.f, 0.f, -0.02f);
	leftHand	   ->setTranslationPhysical(0.f, 0.f, -0.03f);
	rightUpperArm  ->setTranslationPhysical(0.f, 0.f, 0.01f);
	rightLowerArm  ->setTranslationPhysical(0.f, 0.f, 0.02f);
	rightHand	   ->setTranslationPhysical(0.f, 0.f, 0.03f);
	upperLeftLeg   ->setTranslationPhysical(0.f, 0.f, -0.01f);
	lowerLeftLeg   ->setTranslationPhysical(0.f, 0.f, -0.02f);
	upperRightLeg  ->setTranslationPhysical(0.f, 0.f, 0.01f);
	lowerRightLeg  ->setTranslationPhysical(0.f, 0.f, 0.02f);
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