#pragma once

#include <Box2DSuperSprite.h>

class Box2DSprite;
class TextureSampler;
class Particle;
class ParticleSystem : public Box2DSuperSprite{
public:
	float emissionRate;
	unsigned long int emissionAmount;
	float emissionTimer;

	TextureSampler * defaultTex;
	
	Particle * addParticle(glm::vec3 _pos, TextureSampler * _texture = nullptr);

    virtual void update(Step * _step) override;

	ParticleSystem(TextureSampler * _texture, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~ParticleSystem();
};