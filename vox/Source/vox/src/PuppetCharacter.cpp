#pragma once

#include "PuppetCharacter.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include <GameJamCharacter.h>
#include "Box2DWorld.h"
#include "Item.h"
#include "Behaviour.h"
#include <PuppetTexturePack.h>

PuppetCharacter::PuppetCharacter(PuppetTexturePack * _texturePack, bool _ai, Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	texPack(_texturePack),
	ai(_ai),
	canJump(false),
	dead(false),
	deathPending(false),
	targetRoll(0),
	health(1.0f),
	itemToPickup(nullptr),
	heldItem(nullptr),
	itemJoint(nullptr),
	behaviourManager(this),
	score(0.f)
{
	bool defaultTex = false;
	if(texPack == nullptr){
		defaultTex = true;
		texPack = new PuppetTexturePack(
			RaidTheCastleResourceManager::knightRedTorso,
			RaidTheCastleResourceManager::knightRedArm,
			RaidTheCastleResourceManager::knightRedHelmet
		);
	}

	head = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->headTex->width, texPack->headTex->height, texPack->headTex->texture, componentScale);
	face = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->faceTex->width, texPack->faceTex->height, texPack->faceTex->texture, componentScale);
	handLeft = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->handTex->width, texPack->handTex->height, texPack->handTex->texture, componentScale);
	handRight = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->handTex->width, texPack->handTex->height, texPack->handTex->texture, componentScale);

	torso = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->torsoTex->width, texPack->torsoTex->height, texPack->torsoTex->texture, componentScale*0.5f);
	armLeft = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->armTex->width, texPack->armTex->height, texPack->armTex->texture, componentScale*0.5f);
	armRight = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->armTex->width, texPack->armTex->height, texPack->armTex->texture, componentScale*0.5f);
	headgear = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), texPack->headgearTex->width, texPack->headgearTex->height, texPack->headgearTex->texture, componentScale*0.5f);

	components.push_back(&armLeft);
	components.push_back(&armRight);
	components.push_back(&handLeft);
	components.push_back(&handRight);
	components.push_back(&torso);
	components.push_back(&head);
	components.push_back(&face);
	components.push_back(&headgear);

	rootComponent = torso;

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}

	sf.groupIndex = _groupIndex;

	torso->createFixture	 (sf, b2Vec2(0.0f, -1.f), this);
	armLeft->createFixture	 (sf, b2Vec2(0.0f, 0.0f), this);
	armRight->createFixture	 (sf, b2Vec2(0.0f, 0.0f), this);
	handLeft->createFixture  (sf, b2Vec2(0.0f, 0.0f), this);
	handRight->createFixture (sf, b2Vec2(0.0f, 0.0f), this);
	face->createFixture		 (sf, b2Vec2(0.0f, 0.0f), this);
	headgear->createFixture	 (sf, b2Vec2(0.0f, 0.0f), this);
	head->createFixture		 (sf, b2Vec2(0.0f, 0.0f), this);

	b2RevoluteJointDef jth;
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	jth.localAnchorA.Set(0, 0.4f * torso->getCorrectedHeight());
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
	b2WeldJointDef jhh;
	//b2RevoluteJointDef jhh;
	jhh.bodyA = head->body;
	jhh.bodyB = headgear->body;
	jhh.localAnchorA.Set(0, 1.0f * head->getCorrectedHeight());
	jhh.localAnchorB.Set(0, -0.1f * headgear->getCorrectedHeight());
	jhh.collideConnected = false;
	//jhh.enableLimit = true;
	jhh.referenceAngle = 0;
	world->b2world->CreateJoint(&jhh);

	// right arm
	b2RevoluteJointDef jtar;
	jtar.bodyA = torso->body;
	jtar.bodyB = armRight->body;

	jtar.localAnchorA.Set(0.9f * torso->getCorrectedWidth(), 0.3f * torso->getCorrectedHeight());
	jtar.localAnchorB.Set(0, 0.6f * armRight->getCorrectedHeight());

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

	jtal.localAnchorA.Set(-0.9f * torso->getCorrectedWidth(), 0.3f * torso->getCorrectedHeight());
	jtal.localAnchorB.Set(0, 0.6f * armLeft->getCorrectedHeight());

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
	delete texPack;
}

void PuppetCharacter::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Box2DSuperSprite::render(_matrixStack, _renderStack);
}

void PuppetCharacter::update(Step* _step){
	Box2DSuperSprite::update(_step);
	float bodAngle = (torso)->body->GetAngle() + targetRoll;
	(torso->body->SetAngularVelocity(-bodAngle * 10));

	if(!dead){
		//body
		if((torso->body->GetPosition().y < 0)){
			(torso)->applyLinearImpulseUp(250);
		}
		if(itemToPickup != nullptr){
			pickupItem(itemToPickup);
		}
	}else{
		torso->setTranslationPhysical(torso->body->GetPosition().x, 8.0f, torso->transform->translationVector.z);
		torso->body->ApplyForce(b2Vec2(-bodAngle * 50.0f, 0), torso->body->GetWorldCenter(), true);
	}
	behaviourManager.update(_step);
}

void PuppetCharacter::jump(){
	if(canJump){
		std::cout << "jump called for " << &torso << "\n";
		float t = torso->body->GetAngle();
		b2Vec2 p = torso->body->GetWorldPoint(b2Vec2(0, 1));
		//torso->applyLinearImpulse(250*(1-cos(t))*glm::sign(-t), 250*(cos(t)*0.5 + 0.5), p.x, p.y);
		torso->applyLinearImpulseUp(175*(cos(t)*0.5 + 0.5));
		if(torso->body->GetAngle() > 0){
			//torso->applyLinearImpulseLeft(250*(1-cos(t)));
			torso->body->SetLinearVelocity(b2Vec2(-25*(1-cos(t)), torso->body->GetLinearVelocity().y));
		}else{
			torso->body->SetLinearVelocity(b2Vec2(25*(1-cos(t)), torso->body->GetLinearVelocity().y));
		}
	}
	//canJump = false;
}

void PuppetCharacter::action(){
	if(heldItem != nullptr){
		if(itemJoint != nullptr){
			world->b2world->DestroyJoint(itemJoint);
			float t = torso->body->GetAngle();
			(*heldItem->components.at(0))->applyLinearImpulseUp(200);
			if(torso->body->GetAngle() > 0){
				(*heldItem->components.at(0))->applyLinearImpulseLeft(100*(1-cos(t)));
			}else{
				(*heldItem->components.at(0))->applyLinearImpulseRight(100*(1-cos(t)));
			}
			heldItem->thrown = true;
			heldItem->held = false;

			heldItem = nullptr;
			itemJoint = nullptr;
		}
	}
}

void PuppetCharacter::die(){
	dead = true;
	deathPending = false;
	for(Box2DSprite ** c : components) {
		(*c)->body->SetGravityScale(0.0f);
	}
	torso->setTranslationPhysical(torso->body->GetPosition().x, 2.0f, torso->transform->translationVector.z);
}

void PuppetCharacter::unload(){
	Box2DSuperSprite::unload();
}

void PuppetCharacter::load(){
	Box2DSuperSprite::load();
}

void PuppetCharacter::pickupItem(Item * _item){
	if(_item != heldItem){
		if(heldItem != nullptr){
			action();
		}

		// set the item's group index to match character's so that they won't collide anymore (doesn't work?)
		_item->setGroupIndex(this->groupIndex);

		b2WeldJointDef jd;
		jd.bodyA = armRight->body;
		jd.bodyB = (*_item->components.at(0))->body;
		jd.localAnchorA.Set(0, 0);
		jd.localAnchorB.Set(_item->handleX*componentScale, _item->handleY*componentScale);
		jd.collideConnected = false;
		jd.referenceAngle = 0.f;
		itemJoint = (b2WeldJoint *)world->b2world->CreateJoint(&jd);
		heldItem = _item;
		itemToPickup = nullptr;
		_item->held = true;

	}
}

void PuppetCharacter::addToLayeredScene(LayeredScene * _scene, unsigned long int _layer){
	Box2DSuperSprite::addToLayeredScene(_scene, _layer);
	if(itemToPickup != nullptr){
		itemToPickup->addToLayeredScene(_scene, _layer);
	}
}
void PuppetCharacter::setShader(Shader * _shader, bool _configureDefaultAttributes){
	Box2DSuperSprite::setShader(_shader, _configureDefaultAttributes);
	if(itemToPickup != nullptr){
		itemToPickup->setShader(_shader, _configureDefaultAttributes);
	}
}
