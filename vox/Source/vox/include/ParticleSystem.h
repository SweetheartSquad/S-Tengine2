#pragma once

#include <Box2DSuperSprite.h>

class Box2DSprite;
class ParticleSystem : public Box2DSuperSprite{
public:
	float emissionRate;
	unsigned long int emissionAmount;
	float emissionTimer;
	
	void addParticle();

    virtual void update(Step * _step) override;

	ParticleSystem(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~ParticleSystem();
};