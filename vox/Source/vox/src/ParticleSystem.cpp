#pragma once

#include <ParticleSystem.h>

#include <Box2DSprite.h>
#include <Particle.h>
#include <PuppetResourceManager.h>

ParticleSystem::ParticleSystem(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
    Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
    NodeTransformable(new Transform()),
    NodeChild(nullptr)
{

}

ParticleSystem::~ParticleSystem(){

}

void ParticleSystem::update(Step * _step){
	Box2DSuperSprite::update(_step);
    /*for (signed long int i = components.size() - 1; i >= 0; --i){

    }*/
}

void ParticleSystem::addParticle(){
    /*Particle * p = new Particle(world, nullptr, PuppetResourceManager::bush1);
    p->applyLinearImpulse(std::rand(), std::rand(), p->body->GetPosition().x, p->body->GetPosition().y);
    p->setShader(getShader(), false);
    addComponent(p);*/
}