#pragma once

#include "PuppetCharacter.h"
#include <CharacterComponent.h>
#include <Texture.h>
#include <GameJamCharacter.h>
PuppetCharacter::PuppetCharacter(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, bool _ai):
	Character(_world, _categoryBits, _maskBits, _ai),
	NodeRenderable(),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	targetRoll(0.f)
{
	//ComponentTexture * headTex = new ComponentTexture(new Texture("../assets/uv-test.jpg", 1000, 1000, true, true), 1000, 1000);
	//CharacterComponent * head = new CharacterComponent(componentScale, headTex->width, headTex->height, headTex->texture, _world,  b2_dynamicBody, false);
	
	
	GameJamCharacter::texture_packs character = GameJamCharacter::kMICHAEL;
	head = new CharacterComponent(componentScale, GameJamCharacter::headTexPacks[character]->width, GameJamCharacter::headTexPacks[character]->height, GameJamCharacter::headTexPacks[character]->texture, _world, b2_dynamicBody, false);
	
	torso = new CharacterComponent(componentScale, GameJamCharacter::torsoTexPacks[character]->width, GameJamCharacter::torsoTexPacks[character]->height, GameJamCharacter::torsoTexPacks[character]->texture, _world, b2_dynamicBody, false);
	
	//components.push_back(&torso);
	components.push_back(&head);
	
	head->createFixture(groupIndex);



	/*b2PolygonShape tShape;
	tShape.SetAsBox(head->width*std::abs(transform->scaleVector.x)*head->scale*4.f, std::abs(head->height*transform->scaleVector.y)*head->scale*5.f);
	
	b2Fixture * s = head->body->CreateFixture(&tShape, 1);
	s->SetSensor(true);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	s->SetFilterData(sf);*/
}

PuppetCharacter::~PuppetCharacter(){
}

void PuppetCharacter::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Character::render(_matrixStack, _renderStack);
}

void PuppetCharacter::update(Step* _step){
	Character::update(_step);
	//neck
	/*b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
	float angle = neck->GetJointAngle();

	neck->SetMotorSpeed(-angle*360);
	neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));*/

	//body
	float bodAngle = (*components.at(0))->body->GetAngle() - targetRoll;
	(*components.at(0))->body->SetAngularVelocity(-bodAngle*10);
	/*if((*components.at(0))->body->GetPosition().y < 5){
		(*components.at(0))->applyLinearImpulseUp(250);
	}*/
}

void PuppetCharacter::unload(){
	Character::unload();
}

void PuppetCharacter::load(){
	Character::load();
}