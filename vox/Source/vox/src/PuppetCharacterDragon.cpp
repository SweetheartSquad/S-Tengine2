#pragma once

#include <PuppetCharacterDragon.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourManager.h>
#include <Box2DWorld.h>
#include <shader\ShaderComponentHsv.h>
#include <shader\ShaderComponentTint.h>
#include <shader\BaseComponentShader.h>
#include <shader\Shader.h>
#include <RenderOptions.h>
#include <Item.h>
#include <SlayTheDragonResourceManager.h>
#include <PuppetScene.h>
#include <ParticleSystem.h>

PuppetCharacterDragon::PuppetCharacterDragon(bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		SlayTheDragonResourceManager::dragonTorso,
		SlayTheDragonResourceManager::dragonUpperWing,
		SlayTheDragonResourceManager::dragonHead,
		SlayTheDragonResourceManager::dragonMouth, 
		SlayTheDragonResourceManager::dragonLowerWing,
		PuppetResourceManager::face1,
		4.0f
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	fireball(nullptr),
	fireParticles(new ParticleSystem(SlayTheDragonResourceManager::itemFireball, _world, 0, 0, _groupIndex))
{
	behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), this, 10));
	behaviourManager.addBehaviour(new BehaviourAttack(this, 3, PuppetGame::kPLAYER));

	for(auto c : components) {
		(*c)->body->SetGravityScale(0.0f);
	}

	world->b2world->DestroyJoint(handRight->body->GetJointList()->joint);
	world->b2world->DestroyJoint(handLeft->body->GetJointList()->joint);

	// right wing
	b2RevoluteJointDef rhrej;
	rhrej.bodyA = armRight->body;
	rhrej.bodyB = handRight->body;
	rhrej.localAnchorA.Set(armRight->getCorrectedWidth(), -0.2 * armRight->getCorrectedHeight());
	rhrej.localAnchorB.Set(0.6 * handRight->getCorrectedWidth(), 0.8 * handRight->getCorrectedHeight());
	rhrej.collideConnected = false;
	rhrej.enableLimit = true;
	rhrej.referenceAngle = glm::radians(0.f);
	rhrej.lowerAngle = glm::radians(-6.9f);
	rhrej.upperAngle = glm::radians(6.2f);
	world->b2world->CreateJoint(&rhrej);

	// left wing
	b2RevoluteJointDef lhlej;
	lhlej.bodyA = armLeft->body;
	lhlej.bodyB = handLeft->body;
	lhlej.localAnchorA.Set(-armLeft->getCorrectedWidth(), -0.2 * armLeft->getCorrectedHeight());
	lhlej.localAnchorB.Set(-0.6 * handLeft->getCorrectedWidth(), 0.8 * handLeft->getCorrectedHeight());
	lhlej.collideConnected = false;
	lhlej.enableLimit = true;
	lhlej.referenceAngle = glm::radians(0.f);
	lhlej.lowerAngle = glm::radians(-6.2f);
	lhlej.upperAngle = glm::radians(6.9f);
	world->b2world->CreateJoint(&lhlej);
}

PuppetCharacterDragon::~PuppetCharacterDragon(){
	//delete texPack;
	delete fireParticles;
}

void PuppetCharacterDragon::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	// save the current shader settings
	float hue = static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->getHue();
	float sat = static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->getSaturation();
	float red = static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(2))->getRed();

	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(2))->setRed(red + (1 - control) * 3);

	// change the shader settings based on current damage and player id
	if (!ai){
		static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->setHue(float(id) * 0.167f);
	}
	else{
		static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->setSaturation(0.f);
	}

	// lower wing behind upper wing
	handLeft->render(_matrixStack, _renderStack);
	handRight->render(_matrixStack, _renderStack);
	armLeft->render(_matrixStack, _renderStack);
	armRight->render(_matrixStack, _renderStack);
	torso->render(_matrixStack, _renderStack);
	head->render(_matrixStack, _renderStack);
	headgear->render(_matrixStack, _renderStack);
	// This is scary face->render(_matrixStack, _renderStack);

	

	// revert the shader settings
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->setHue(hue);
	static_cast<ShaderComponentHsv *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(1))->setSaturation(sat);

	static_cast<ShaderComponentTint *>(static_cast<BaseComponentShader *>(_renderStack->shader)->components.at(2))->setRed(red);

	if(fireball != nullptr){
		fireParticles->addParticle(fireball->rootComponent->getPos(false));
	}
	fireParticles->setShader(getShader(), true);
	fireParticles->render(_matrixStack, _renderStack);
}

void PuppetCharacterDragon::update(Step * _step){
	PuppetCharacter::update(_step);
	b2Vec2 center = torso->body->GetWorldCenter();
	torso->applyForce(torso->body->GetAngle() * -5000.0f, 0.0f, center.x, center.y);
	torso->body->SetTransform(b2Vec2(torso->body->GetPosition().x, 60.0f), torso->body->GetAngle());
	if(ai){
		if(behaviourManager.behaviours.at(1)->active){
			behaviourManager.behaviours.at(0)->active = false;
		}else{
			behaviourManager.behaviours.at(0)->active = true;
		}
	}
	fireParticles->update(_step);
}
void PuppetCharacterDragon::action(){
	if(heldItem != nullptr){
		if(itemJoint != nullptr){
			/*try{
				static_cast<PuppetScene *>(scene)->particleSystem->addParticle(PuppetResourceManager::dustParticle, fireball->getPos(false));
			}catch (std::exception){
				fireball = nullptr;
			}*/
			if (fireball == nullptr){
				fireball = heldItem->getProjectile();
				if (fireball == heldItem){
					heldItem = nullptr;
					itemJoint = nullptr;
					itemToPickup = nullptr;
				}
				float t = rootComponent->body->GetAngle();
				fireball->rootComponent->body->SetTransform(fireball->rootComponent->body->GetPosition(), t);
				fireball->rootComponent->applyLinearImpulseDown(50);
				if(rootComponent->body->GetAngle() > 0){
					fireball->rootComponent->applyLinearImpulseLeft(50 * (1 - cos(t)));
				}else{
					fireball->rootComponent->applyLinearImpulseRight(50 * (1 - cos(t)));
				}
			}
		}
	}
}

void PuppetCharacterDragon::pickupItem(Item * _item){
	if(_item != heldItem){
		if(heldItem != nullptr){
			action();
		}

		// set the item's group index to match character's so that they won't collide anymore (doesn't work?)
		_item->setGroupIndex(this->groupIndex);

		b2WeldJointDef jd;
		jd.bodyA = face->body;
		jd.bodyB = (*_item->components.at(0))->body;
		jd.localAnchorA.Set(0.f, -0.9f * face->getCorrectedHeight());
		jd.localAnchorB.Set(_item->handleX * componentScale, _item->handleY * componentScale);
		jd.collideConnected = false;
		jd.referenceAngle = glm::radians(-90.f);
		jd.dampingRatio = 0;
		itemJoint = _item->playerJoint = (b2WeldJoint *)world->b2world->CreateJoint(&jd);
		heldItem = _item;
		itemToPickup = nullptr;
		_item->held = true;
		_item->owner = this;
	}
}