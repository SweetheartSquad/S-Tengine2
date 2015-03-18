#pragma once

#include <Particle.h>

Particle::Particle(Box2DWorld * _world, Shader * _shader, Texture * _texture) :
Box2DSprite(_world, b2_dynamicBody, false, _shader, transform, 1, 1, _texture),
NodeTransformable(new Transform()),
NodeChild(nullptr),
life(1),
age(0),
alive(true)
{

}
void Particle::update(Step * _step){
    Box2DSprite::update(_step);
    age += 0.001f;
    if (age > life){
        alive = false;
    }
}

Particle::~Particle(){

}