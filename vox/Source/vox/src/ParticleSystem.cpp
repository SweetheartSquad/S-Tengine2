#pragma once

#include <ParticleSystem.h>

#include <Box2DSprite.h>
#include <Particle.h>
#include <PuppetResourceManager.h>

ParticleSystem::ParticleSystem(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
    Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
    NodeTransformable(new Transform()),
    NodeChild(nullptr),
    emissionAmount(0),
    emissionRate(0),
	emissionTimer(0)
{
}

ParticleSystem::~ParticleSystem(){

}

void ParticleSystem::update(Step * _step){
	Box2DSuperSprite::update(_step);
	emissionTimer += _step->deltaTime;
	while (emissionTimer > emissionRate){
		emissionTimer -= emissionRate;
		for (unsigned long int i = 0; i < emissionAmount; ++i){
			addParticle();
		}
	}
    for (signed long int i = components.size() - 1; i >= 0; --i){
		Particle * p = static_cast<Particle *>(*components.at(i));
		if (!p->alive){
			delete p;
			delete components.at(i);
			components.erase(components.begin() + i);
		}
    }
}

void ParticleSystem::addParticle(){
    Box2DSprite ** test = new Box2DSprite*[1];
    Particle * p = new Particle(world, PuppetResourceManager::head1);
    test[0] = p;
    p->setTranslationPhysical(20, 20, 0);
    p->applyLinearImpulse(std::rand(), std::rand(), p->body->GetPosition().x, p->body->GetPosition().y);
    p->setShader(getShader(), true);
    //p->createFixture(b2Filter());
    components.push_back(test);
}