#pragma once

#include "Character.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"

Character::Character(Box2DWorld * _world)
	:Entity(new Transform()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world)
{
}

Character::~Character(){
}

void Character::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Entity::render(_matrixStack, _renderStack);
	/*torso->render(_matrixStack, _renderStack);
	leftUpperArm->render(_matrixStack, _renderStack);
	leftLowerArm->render(_matrixStack, _renderStack);
	leftHand->render(_matrixStack, _renderStack);
	rightUpperArm->render(_matrixStack, _renderStack);
	rightLowerArm->render(_matrixStack, _renderStack);
	rightHand->render(_matrixStack, _renderStack);
	upperLeftLeg->render(_matrixStack, _renderStack);
	lowerLeftLeg->render(_matrixStack, _renderStack);
	upperRightLeg->render(_matrixStack, _renderStack);
	lowerRightLeg->render(_matrixStack, _renderStack);
	*/
}

void Character::update(Step* _step){
	Entity::update(_step);
	/*torso->update(_step);
	leftUpperArm->update(_step);
	leftLowerArm->update(_step);
	leftHand->update(_step);
	rightUpperArm->update(_step);
	rightLowerArm->update(_step);
	rightHand->update(_step);
	upperLeftLeg->update(_step);
	lowerLeftLeg->update(_step);
	upperRightLeg->update(_step);
	lowerRightLeg->update(_step);*/
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