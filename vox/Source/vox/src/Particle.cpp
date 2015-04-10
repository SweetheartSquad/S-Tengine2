#pragma once

#include <Particle.h>
#include <Box2DSuperSprite.h>
#include <Easing.h>

Particle::Particle(Box2DWorld * _world, TextureSampler * _texture, Transform * _transform) :
	Box2DSprite(_world, _texture, b2_dynamicBody, false, nullptr, _transform),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	life(1),
	age(0),
	alive(true),
	startSize(1),
	deltaSize(-1)
{
	b2Filter f;
	f.categoryBits = 0;
	f.groupIndex = 0;
	f.maskBits = 0;
    createFixture(f, b2Vec2(0,0), this);
}
void Particle::update(Step * _step){
    Box2DSprite::update(_step);
    age += _step->deltaTime;
    if (age > life){
        alive = false;
	}else{
		glm::vec3 sv = transform->getScaleVector();
		sv.x = Easing::easeInOutCubic(age, startSize, deltaSize, life);//1.f - age / life;
		sv.y = sv.x;
		transform->scale(sv, false);
	}
}

Particle::~Particle(){

}