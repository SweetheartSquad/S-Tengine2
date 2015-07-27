#pragma once

#include <ParticleSystem.h>

#include <Box2DSprite.h>
#include <Particle.h>
#include <Box2DWorld.h>
#include <Texture.h>

ParticleSystem::ParticleSystem(Texture * _texture, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
    Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
    emissionAmount(0),
    emissionRate(0),
	emissionTimer(0),
	defaultTex(_texture)
{
	++defaultTex->referenceCount;
}

ParticleSystem::~ParticleSystem(){
	defaultTex->decrementAndDelete();
}

void ParticleSystem::update(Step * _step){
	Box2DSuperSprite::update(_step);
	if (emissionRate > 0){
		emissionTimer += _step->deltaTime;
		while (emissionTimer > emissionRate){
			emissionTimer -= emissionRate;
			for (unsigned long int i = 0; i < emissionAmount; ++i){
				addParticle(glm::vec3(0, 0, 0));
			}
		}
	}
	for (signed long int i = components.size() - 1; i >= 0; --i){
		Particle * p = static_cast<Particle *>(*components.at(i));
		if (!p->alive){
			childTransform->removeChild(p->parents.at(0));
			delete p->parents.at(0);
			delete components.at(i);
			components.erase(components.begin() + i);
		}
	}
}

Particle * ParticleSystem::addParticle(glm::vec3 _pos, Texture * _texture){
	if(_texture == nullptr){
		_texture = defaultTex;
	}
    Box2DSprite ** test = new Box2DSprite*[1];
	Particle * p = new Particle(world, _texture);
	p->startSize = componentScale;
	p->deltaSize = -componentScale;
	childTransform->addChild(p)->scale(p->startSize, false);
    test[0] = p;
	p->setTranslationPhysical(_pos.x, _pos.y, _pos.z);
	float mass = p->body->GetMass();
	p->applyLinearImpulseToCenter((std::rand() % 20 - 10)*mass, (std::rand() % 20 - 10)*mass);
	p->applyAngularImpulse((std::rand() % 20 - 10)*mass);
    p->setShader(shader, true);
    components.push_back(test);
	return p;
}