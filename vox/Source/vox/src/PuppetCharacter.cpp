#pragma once

#include "PuppetCharacter.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include <GameJamCharacter.h>
#include "Box2DWorld.h"
PuppetCharacter::PuppetCharacter(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, bool _ai):
	Box2DSuperSprite(_world, _categoryBits, _maskBits),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	targetRoll(0),
	ai(ai)
{
	
	GameJamCharacter::texture_packs character = GameJamCharacter::kKNIGHT;
	head = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::headTexPacks[character]->width, GameJamCharacter::headTexPacks[character]->height, GameJamCharacter::headTexPacks[character]->texture, componentScale);
	
	torso = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::torsoTexPacks[character]->width, GameJamCharacter::torsoTexPacks[character]->height, GameJamCharacter::torsoTexPacks[character]->texture, componentScale);
	armLeft = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::upperArmTexPacks[character]->width, GameJamCharacter::upperArmTexPacks[character]->height, GameJamCharacter::upperArmTexPacks[character]->texture, componentScale);
	armRight = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::upperArmTexPacks[character]->width, GameJamCharacter::upperArmTexPacks[character]->height, GameJamCharacter::upperArmTexPacks[character]->texture, componentScale);
	handLeft = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::handTexPacks[character]->width, GameJamCharacter::handTexPacks[character]->height, GameJamCharacter::handTexPacks[character]->texture, componentScale);
	handRight = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), GameJamCharacter::handTexPacks[character]->width, GameJamCharacter::handTexPacks[character]->height, GameJamCharacter::handTexPacks[character]->texture, componentScale);
	

	TextureSampler * faceTex = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Face1.png", 512,512, true, true), 67, 72);
	TextureSampler * helmetTex = new TextureSampler(new Texture("../assets/hurly-burly/KnightAssets/Helmet1.png", 512,512, true, true), 114, 165);
	face = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), faceTex->width, faceTex->height, faceTex->texture, componentScale);
	headgear = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), helmetTex->width, helmetTex->height, helmetTex->texture, componentScale);

	components.push_back(&armLeft);
	components.push_back(&armRight);
	components.push_back(&handLeft);
	components.push_back(&handRight);
	components.push_back(&torso);
	components.push_back(&head);
	components.push_back(&face);
	components.push_back(&headgear);
	
	/*b2CircleShape tShape;
	tShape.m_radius = glm::length(glm::vec2(transform->scaleVector.x,transform->scaleVector.y));
	tShape.m_p = b2Vec2(0,0);
	b2Fixture * s = torso->body->CreateFixture(&tShape, 1);*/
	
	
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

	for(Box2DSprite ** c : components){
		(*c)->createFixture(groupIndex);
	}
	
	// neck
	b2RevoluteJointDef jth;
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	jth.localAnchorA.Set(0, 0.9f * torso->getCorrectedHeight());
	jth.localAnchorB.Set(0, -0.9f * head->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.enableMotor = true;
	jth.maxMotorTorque = 0;
	jth.motorSpeed = 0;
	jth.referenceAngle = 0;
	jth.lowerAngle = glm::radians(-15.f);
	jth.upperAngle = glm::radians(15.f);
	world->b2world->CreateJoint(&jth);
	
	// face
	b2RevoluteJointDef jhf;
	jhf.bodyA = head->body;
	jhf.bodyB = face->body;
	jhf.localAnchorA.Set(0, 0);
	jhf.localAnchorB.Set(0, 0);
	jhf.collideConnected = false;
	jhf.enableLimit = true;
	jhf.referenceAngle = 0;
	world->b2world->CreateJoint(&jhf);
	// headgear
	b2RevoluteJointDef jhh;
	jhh.bodyA = head->body;
	jhh.bodyB = headgear->body;
	jhh.localAnchorA.Set(0, 1.0f * head->getCorrectedHeight());
	jhh.localAnchorB.Set(0, -0.1f * headgear->getCorrectedHeight());
	jhh.collideConnected = false;
	jhh.enableLimit = true;
	jhh.referenceAngle = 0;
	world->b2world->CreateJoint(&jhh);

	
	// right arm
	b2RevoluteJointDef jtar;
	jtar.bodyA = torso->body;
	jtar.bodyB = armRight->body;
	jtar.localAnchorA.Set(0.9 * torso->getCorrectedWidth(), 0.8f * torso->getCorrectedHeight());
	jtar.localAnchorB.Set(0, 0.6 * armRight->getCorrectedHeight());
	jtar.collideConnected = false;
	jtar.enableLimit = true;
	jtar.enableMotor = true;
	jtar.maxMotorTorque = 0;
	jtar.motorSpeed = 0;
	jtar.referenceAngle = glm::radians(90.f);
	jtar.lowerAngle = glm::radians(-10.f);
	jtar.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&jtar);

	
	// left arm
	b2RevoluteJointDef jtal;
	jtal.bodyA = torso->body;
	jtal.bodyB = armLeft->body;
	jtal.localAnchorA.Set(-0.9 * torso->getCorrectedWidth(), 0.8f * torso->getCorrectedHeight());
	jtal.localAnchorB.Set(0, 0.6 * armLeft->getCorrectedHeight());
	jtal.collideConnected = false;
	jtal.enableLimit = true;
	jtal.enableMotor = true;
	jtal.maxMotorTorque = 0;
	jtal.motorSpeed = 0;
	jtal.referenceAngle = glm::radians(-90.f);
	jtal.lowerAngle = glm::radians(-10.f);
	jtal.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&jtal);

	

	// right hand
	b2RevoluteJointDef rhrej;
	rhrej.bodyA = armRight->body;
	rhrej.bodyB = handRight->body;
	rhrej.localAnchorA.Set(0.f, -0.9f * armRight->getCorrectedHeight());
	rhrej.localAnchorB.Set(0.f, 0.f);
	rhrej.collideConnected = false;
	rhrej.enableLimit = true;
	rhrej.referenceAngle = glm::radians(0.f);
	rhrej.lowerAngle = glm::radians(-20.f);
	rhrej.upperAngle = glm::radians(20.f);
	world->b2world->CreateJoint(&rhrej);
	
	// left hand
	b2RevoluteJointDef lhlej;
	lhlej.bodyA = armLeft->body;
	lhlej.bodyB = handLeft->body;
	lhlej.localAnchorA.Set(0.f, -0.9f * armLeft->getCorrectedHeight());
	lhlej.localAnchorB.Set(-0.f, 0.f);
	lhlej.collideConnected = false;
	lhlej.enableLimit = true;
	lhlej.referenceAngle = glm::radians(0.f);
	lhlej.lowerAngle = glm::radians(-20.f);
	lhlej.upperAngle = glm::radians(20.f);
	world->b2world->CreateJoint(&lhlej);

	// flip left side
	armLeft->transform->scale(-1, 1, 1);
	handLeft->transform->scale(-1, 1, 1); 
}

PuppetCharacter::~PuppetCharacter(){
}

void PuppetCharacter::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Box2DSuperSprite::render(_matrixStack, _renderStack);
}

void PuppetCharacter::update(Step* _step){
	Box2DSuperSprite::update(_step);
	//neck
	//b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
	//float angle = neck->GetJointAngle();

	//neck->SetMotorSpeed(-angle*360);
	//neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));

	//body
	float bodAngle = (torso)->body->GetAngle() + targetRoll;
	(torso->body->SetAngularVelocity(-bodAngle * 10));
	if((torso->body->GetPosition().y < 0)){
		(torso)->applyLinearImpulseUp(250);
	}
	//torso->body->SetTransform(torso->body->GetPosition(), targetRoll);
	
	//headgear->body->SetTransform(head->body->GetPosition(), head->body->GetAngle());
	//face->body->SetTransform(head->body->GetPosition(), head->body->GetAngle());
}

void PuppetCharacter::unload(){
	Box2DSuperSprite::unload();
}

void PuppetCharacter::load(){
	Box2DSuperSprite::load();
}