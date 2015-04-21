#pragma once

#include <Box2DMeshEntity.h>

class Box2DWorld;
class Material;

class LD32_Monster : public Box2DMeshEntity{
public:
	float spawnEnemy;
	

	LD32_Monster(Box2DWorld * _world);
	~LD32_Monster();
	
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void setShader(Shader * _shader, bool _def) override;
};