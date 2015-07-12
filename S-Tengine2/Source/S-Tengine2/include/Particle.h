#pragma once

#include <Box2DSprite.h>

class TextureSampler;

class Particle : public Box2DSprite{
public:
    float age;
    float life;
	float * prop;
    bool alive;

	float startSize;
	float deltaSize;

    explicit Particle(Box2DWorld * _world, TextureSampler * _texture = nullptr);
    virtual void update(Step * _step) override;
};