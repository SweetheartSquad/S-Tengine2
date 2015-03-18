#pragma once

#include <Box2DSuperSprite.h>

class ParticleSystem : public Box2DSuperSprite{
public:
	float emmissionRate;
	float emissionAmount;
	
	void addParticle();

    virtual void update(Step * _step) override;

	ParticleSystem(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~ParticleSystem();
};