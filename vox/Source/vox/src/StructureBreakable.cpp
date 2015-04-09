#pragma once

#include <StructureBreakable.h>
#include <ParticleSystem.h>
#include <PuppetResourceManager.h>
#include <Box2DSprite.h>
#include <Particle.h>
#include <NumberUtils.h>

StructureBreakable::StructureBreakable(float _maxHealth, Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	health(_maxHealth),
	maxHealth(_maxHealth),
	state(kNORMAL),
	dead(false),
	scattering(false),
	destroy(false)
{
	particleSystem = new ParticleSystem(PuppetResourceManager::dustParticle, world, 0, 0, 0);
	addChild(particleSystem);
	particleSystem->emissionRate = -1;
	particleSystem->emissionAmount = 0;
}

StructureBreakable::~StructureBreakable(){
	delete particleSystem;
}

void StructureBreakable::update(Step* _step){
	Structure::update(_step);

	if(dead){
		for (signed long int j = 0; j < std::rand() % 20 + 100; ++j){
			Particle * p = particleSystem->addParticle(rootComponent->getPos(false));
			p->setTranslationPhysical(vox::NumberUtils::randomFloat(getPos().x - rootComponent->getCorrectedWidth(), getPos().x + rootComponent->getCorrectedWidth()), vox::NumberUtils::randomFloat(getPos().y - rootComponent->getCorrectedHeight(), getPos().y + rootComponent->getCorrectedHeight()), 0, true);
			p->applyLinearImpulse(vox::NumberUtils::randomFloat(-750, 750), vox::NumberUtils::randomFloat(1000, 1500), p->body->GetPosition().x, p->body->GetPosition().y);
			
		}
		dead = false;
		scattering = true;
	}

	if(scattering){
		if(particleSystem->components.size() == 0){

			scattering = false;
			destroy = true;
		}
	}
}

void StructureBreakable::takeDamage(float _damage){
	if(state != kDEAD){
		health -= _damage;
		if(health <= 0.f){
			state = kDEAD;
			health = 0;
			dead = true;
		}else if(health <= maxHealth * 0.5f){
			state = kDAMAGED;
		}else{
			state = kNORMAL;
			if(health >= maxHealth){
				health = maxHealth;
			}
		}
	}
}

void StructureBreakable::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	Structure::setShader(_shader, _configureDefaultVertexAttributes);
	particleSystem->setShader(_shader, _configureDefaultVertexAttributes);
}