#pragma once

#include <Box2DMeshEntity.h>

class Material;

class LD32_Player : public Box2DMeshEntity{
public:
	
	Material * playerMat;
	std::vector<Box2DMeshEntity *> things;

	std::vector<b2PrismaticJoint *> joints;
	int hits;
	bool deathPending;
	bool won;

	LD32_Player(Box2DWorld * _world);

	void update(Step * _step);
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void setShader(Shader * _shader, bool _def) override;
};