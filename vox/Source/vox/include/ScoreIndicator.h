#pragma once

#include <Sprite.h>

class ParticleSystem;
class Box2DWorld;

class ScoreIndicator : public Sprite{
public:
	ParticleSystem * scoreParticles;
	ScoreIndicator(unsigned long int _id, Box2DWorld * _world);
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step * _step) override;
	void setShader(Shader * _shader, bool _configure) override;
	unsigned long int id;
};