#pragma once

#include <Lever.h>

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <RapunzelResourceManager.h>
#include <Item.h>
#include <Rapunzel.h>
#include <PuppetCharacterGuard.h>
#include <BehaviourManager.h>
#include <BehaviourAttack.h>
#include <BehaviourPatrol.h>
#include <shader\BaseComponentShader.h>
#include <StructureBoxingGlove.h>

glm::vec3 Lever::towerPos;

Lever::Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	StructureInteractable(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.005f;
	type = -1;

	rootComponent = base = new Box2DSprite(_world, RapunzelResourceManager::leverBase, b2_staticBody, false, nullptr, new Transform(), componentScale);
	handle = new Box2DSprite(_world, RapunzelResourceManager::leverHandle, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
	
	components.push_back(&base);
	components.push_back(&handle);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	for(Box2DSprite ** c : components){
		b2Fixture * f = (*c)->createFixture(sf);
		f->SetSensor(true);
	}

	setUserData(this);

	base->setTranslationPhysical(0.f, base->getCorrectedHeight(), 0.f);
	
	handle->body->GetFixtureList()->SetDensity(10.f);

	// handle
	b2RevoluteJointDef jth;
	jth.bodyA = base->body;
	jth.bodyB = handle->body;
	jth.localAnchorA.Set(0.f, 0.9f * base->getCorrectedHeight());
	jth.localAnchorB.Set(0.f, -0.9f * handle->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.referenceAngle = 0.f;
	jth.lowerAngle = glm::radians(-80.f);
	jth.upperAngle = glm::radians(0.f);
	world->b2world->CreateJoint(&jth);
}

Lever::~Lever(){
}

void Lever::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	StructureInteractable::render(_matrixStack, _renderOptions);
}

void Lever::evaluateState(){
	b2RevoluteJoint * jk = (b2RevoluteJoint *)base->body->GetJointList()->joint;
	float angle = jk->GetJointAngle();
	
	handle->body->SetAngularVelocity(2.5);

	if(triggering){
		handle->body->SetAngularVelocity(-20);
		if(angle <= glm::radians(-75.f)){
			triggering = false;
		}
	}else{
		if(angle >= -0.0001f){
			ready = true;
		}
	}
}

void Lever::actuallyInteract(){
	std::cout << type << std::endl;
	Rapunzel * ps = static_cast<Rapunzel *>(scene);
	if(type == 1){
		Item * projectile = new Item(true, world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kBOUNDARY, groupIndex);
	
		Box2DSprite ** test = new Box2DSprite*[1];
		test[0] = projectile->rootComponent = new Box2DSprite(world, RapunzelResourceManager::itemSpear, b2_dynamicBody, false, nullptr, new Transform(), componentScale);
		projectile->rootComponent->body->SetTransform(projectile->rootComponent->body->GetPosition(), glm::radians(80.f));
		projectile->components.push_back(test);

		projectile->translateComponents(towerPos - glm::vec3(0.f, 5.f, 0.f));

		b2Filter sf;
		sf.categoryBits = projectile->categoryBits;
		if(projectile->maskBits != (int16)-1){
			sf.maskBits = projectile->maskBits;
		}else{
			sf.maskBits = 0;
		}
		sf.groupIndex = projectile->groupIndex;
	
		for(Box2DSprite ** c : projectile->components){
			(*c)->createFixture(sf);
			(*c)->body->GetFixtureList()->SetDensity(0.01f);
			(*c)->body->ResetMassData();
		}

		projectile->setUserData(projectile);
		projectile->setUserData(projectile);
		projectile->rootComponent->applyLinearImpulseRight(-50.f);

		ps->addChild(projectile, 1);
		ps->items.push_back(projectile);
		//projectile->addToLayeredScene(static_cast<PuppetScene *>(scene), 1);
		projectile->setShader((Shader *)ps->shader, true);
	}else if(type == 2){
		ps->glove->punch();
	}else if(type == 3){
		PuppetCharacterGuard * g = new PuppetCharacterGuard(true, world, PuppetGame::kPLAYER, PuppetGame::kGROUND | PuppetGame::kSTRUCTURE | PuppetGame::kITEM | PuppetGame::kPLAYER | PuppetGame::kBEHAVIOUR | PuppetGame::kBOUNDARY, -10);
		
		g->behaviourManager->addBehaviour(new BehaviourPatrol(glm::vec3(50, 0, 0), glm::vec3(100, 0, 0), g, 10));
		g->behaviourManager->addBehaviour(new BehaviourAttack(g, 3, PuppetGame::kPLAYER));
		g->ai = true;
		g->addToLayeredScene(ps, 1);
		ps->addChild(g, 1);
		g->setShader(ps->shader, true);
		g->translateComponents(towerPos + glm::vec3(-1.f, -1.f, 0.f));
	}else{
		throw "um";
	}
}