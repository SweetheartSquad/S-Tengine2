#pragma once

#include <Box2DSprite.h>

class Particle : public Box2DSprite{
public:
    float age;
    float life;
    bool alive;

    explicit Particle(Box2DWorld * _world, Shader * _shader = nullptr, Texture * _texture = nullptr);
    ~Particle();

    virtual void update(Step * _step) override;
};