#pragma once

#include <Box2DMeshEntity.h>

class Box2DWorld;
class Material;

class LD32_Donut : public Box2DMeshEntity{
public:
	static MeshInterface * donutTopMesh;
	static MeshInterface * donutBotMesh;
	static Material * donutMat;
	
	MeshEntity * bot;

	bool justHit;
	float lastHit;

	float rotSpeed;

	LD32_Donut(Box2DWorld * _world);
	~LD32_Donut();

	void hit();
	
	void update(Step * _step) override;
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void setShader(Shader * _shader, bool _def) override;
};