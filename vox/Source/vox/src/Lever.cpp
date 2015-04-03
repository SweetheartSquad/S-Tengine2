#pragma once

#include <Lever.h>

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <RapunzelResourceManager.h>
#include <Item.h>
#include <PuppetScene.h>

Lever::Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	StructureInteractable(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.025f;
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

	if(type == 1){
		Item * projectile = new Item(true, world, categoryBits, maskBits, groupIndex);
	
		Box2DSprite ** test = new Box2DSprite*[1];
		test[0] = projectile->rootComponent = new Box2DSprite(world, RapunzelResourceManager::itemScimitar, b2_dynamicBody, false, nullptr, new Transform(), componentScale);

		projectile->components.push_back(test);

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
		projectile->rootComponent->applyLinearImpulseRight(100.f);

		static_cast<PuppetScene *>(scene)->addChild(projectile, 1);
		static_cast<PuppetScene *>(scene)->items.push_back(projectile);
		//projectile->addToLayeredScene(static_cast<PuppetScene *>(scene), 1);
		projectile->setShader((Shader *)static_cast<PuppetScene *>(scene)->shader, true);
	}
}