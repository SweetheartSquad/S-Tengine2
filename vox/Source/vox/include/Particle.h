#pragma once

#include <Box2DSprite.h>

class TextureSampler;

class Particle : public Box2DSprite{
public:
    float age;
    float life;
    bool alive;

    explicit Particle(Box2DWorld * _world, TextureSampler * _texture = nullptr, Transform * _transform = new Transform());
    ~Particle();

    virtual void update(Step * _step) override;
};