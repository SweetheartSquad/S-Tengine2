#pragma once

#include "PuppetCharacter.h"
#include <CharacterComponent.h>
#include <Texture.h>
#include <GameJamCharacter.h>
#include "Box2DWorld.h"
PuppetCharacter::PuppetCharacter(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, bool _ai):
	Character(_world, _categoryBits, _maskBits, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	targetRoll(0)
{
	//ComponentTexture * headTex = new ComponentTexture(new Texture("../assets/uv-test.jpg", 1000, 1000, true, true), 1000, 1000);
	//CharacterComponent * head = new CharacterComponent(componentScale, headTex->width, headTex->height, headTex->texture, _world,  b2_dynamicBody, false);
	
	
	GameJamCharacter::texture_packs character = GameJamCharacter::kMICHAEL;

	head = new CharacterComponent(componentScale, GameJamCharacter::headTexPacks[character]->width, GameJamCharacter::headTexPacks[character]->height, GameJamCharacter::headTexPacks[character]->texture, _world, b2_dynamicBody, false);
	torso = new CharacterComponent(componentScale, GameJamCharacter::torsoTexPacks[character]->width, GameJamCharacter::torsoTexPacks[character]->height, GameJamCharacter::torsoTexPacks[character]->texture, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, GameJamCharacter::upperArmTexPacks[character]->width, GameJamCharacter::upperArmTexPacks[character]->height, GameJamCharacter::upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	leftHand  = new CharacterComponent(componentScale, GameJamCharacter::lowerArmTexPacks[character]->width, GameJamCharacter::lowerArmTexPacks[character]->height, GameJamCharacter::lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, GameJamCharacter::upperArmTexPacks[character]->width, GameJamCharacter::upperArmTexPacks[character]->height, GameJamCharacter::upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightHand = new CharacterComponent(componentScale, GameJamCharacter::lowerArmTexPacks[character]->width, GameJamCharacter::lowerArmTexPacks[character]->height, GameJamCharacter::lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);

	components.push_back(&torso);
	components.push_back(&head);
	components.push_back(&leftUpperArm);
	components.push_back(&leftHand);
	components.push_back(&rightUpperArm);
	components.push_back(&rightHand);
	
	/*b2CircleShape tShape;
	tShape.m_radius = glm::length(glm::vec2(transform->scaleVector.x,transform->scaleVector.y));
	tShape.m_p = b2Vec2(0,0);*/
	
	
	/*b2PolygonShape tShape;
	tShape.SetAsBox(head->width*std::abs(transform->scaleVector.x)*head->scale, std::abs(head->height*transform->scaleVector.y)*head->scale);
	
	b2Fixture * s = head->body->CreateFixture(&tShape, 1);
	s->SetSensor(true);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	s->SetFilterData(sf);*/

	head->createFixture(groupIndex);
	head->setTranslationPhysical(0.f, 0.f, 0);
	torso->createFixture(groupIndex);
	torso->setTranslationPhysical(0.f, 0.f, 0);

	float correctedTorsoWidth = torso->getCorrectedWidth();
	float correctedTorsoHeight = torso->getCorrectedHeight();
	
	// neck
	b2RevoluteJointDef jth;
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	jth.localAnchorA.Set(0, 1.0f * torso->getCorrectedHeight());
	jth.localAnchorB.Set(0, 1.0f * -head->getCorrectedHeight());
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
	leftUpperArm->createFixture(groupIndex);
	b2RevoluteJointDef latj;
	latj.bodyA = torso->body;
	latj.bodyB = leftUpperArm->body;
	latj.localAnchorA.Set(-ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	latj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	latj.collideConnected = false;
	latj.enableLimit = true;
	latj.referenceAngle = glm::radians(0.f);
	latj.lowerAngle = glm::radians(-120.f);
	latj.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&latj);
	

	// right shoulder
	rightUpperArm->createFixture(groupIndex);
	b2RevoluteJointDef ratj;
	ratj.bodyA = torso->body;
	ratj.bodyB = rightUpperArm->body;
	ratj.localAnchorA.Set(ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	ratj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	ratj.collideConnected = false;
	ratj.enableLimit = true;
	ratj.referenceAngle = glm::radians(0.f);
	ratj.lowerAngle = glm::radians(-10.f);
	ratj.upperAngle = glm::radians(120.f);
	world->b2world->CreateJoint(&ratj);
	
	
	// left hand
	leftHand->createFixture(groupIndex);
	b2RevoluteJointDef lelsj;
	lelsj.bodyA = leftUpperArm->body;
	lelsj.bodyB = leftHand->body;
	lelsj.localAnchorA.Set(-ratioX_shoulder_to_elbow * leftUpperArm->getCorrectedWidth(), -ratioY_shoulder_to_elbow * leftUpperArm->getCorrectedHeight());
	lelsj.localAnchorB.Set(-ratioX_elbow_to_shoulder * leftHand->getCorrectedWidth(), ratioY_elbow_to_shoulder * leftHand->getCorrectedHeight());
	lelsj.collideConnected = false;
	lelsj.enableLimit = true;
	lelsj.referenceAngle = glm::radians(0.f);
	lelsj.lowerAngle = glm::radians(-10.f);
	lelsj.upperAngle = glm::radians(90.f);
	world->b2world->CreateJoint(&lelsj);

	// right elbow
	rightHand->createFixture(groupIndex);
	b2RevoluteJointDef rersj;
	rersj.bodyA = rightUpperArm->body;
	rersj.bodyB = rightHand->body;
	rersj.localAnchorA.Set(-ratioX_shoulder_to_elbow * rightUpperArm->getCorrectedWidth(), -ratioY_shoulder_to_elbow * rightUpperArm->getCorrectedHeight());
	rersj.localAnchorB.Set(ratioX_elbow_to_shoulder * rightHand->getCorrectedWidth(), ratioY_elbow_to_shoulder * rightHand->getCorrectedHeight());
	rersj.collideConnected = false;
	rersj.enableLimit = true;
	rersj.referenceAngle = glm::radians(0.f);
	rersj.lowerAngle = glm::radians(-90.f);
	rersj.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&rersj);

}

PuppetCharacter::~PuppetCharacter(){
}

void PuppetCharacter::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Character::render(_matrixStack, _renderStack);
}

void PuppetCharacter::update(Step* _step){
	Character::update(_step);
	//neck
	//b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
	//float angle = neck->GetJointAngle();

	//neck->SetMotorSpeed(-angle*360);
	//neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));

	//body
	float bodAngle = (*components.at(0))->body->GetAngle() - targetRoll;
	(*components.at(0))->body->SetAngularVelocity(-bodAngle*10);
	if((*components.at(0))->body->GetPosition().y < 5){
		(*components.at(0))->applyLinearImpulseUp(250);
	}
}

void PuppetCharacter::unload(){
	Character::unload();
}

void PuppetCharacter::load(){
	Character::load();
}