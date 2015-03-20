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
	Particle * p = new Particle(world, PuppetResourceManager::dustParticle);
    test[0] = p;
	p->setTranslationPhysical(20, 20, 0);
	float mass = p->body->GetMass();
	b2Vec2 pos = p->body->GetPosition();
	p->applyLinearImpulse((std::rand() % 20 - 10)*mass, (std::rand() % 20 - 10)*mass, pos.x, pos.y);
	p->applyAngularImpulse((std::rand() % 20 - 10)*mass);
    p->setShader(getShader(), true);
    //p->createFixture(b2Filter());
    components.push_back(test);
}