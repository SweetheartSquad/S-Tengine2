#pragma once

#include <iostream>
#include "Scene.h"
#include "Light.h"
#include "Cube.h"
#include "Vox.h"

class Texture;
class Shader;
class Material;
class MeshEntity;
class VoxelJoint;

class MainScene : public Scene{
public:

	Cube * cube;
	Cube * cube2;
	Cube * cube3;
	Cube * cube4;

	Texture * tex;
	Texture * voxTex;

	Material * mat;
	Material * bMat;

	Shader * texShader;
	Shader * phongShader;
	Shader * blinnShader;
	Shader * voxShader;

	StandardFrameBuffer * frameBuffer;

	RenderSurface * renderSurface;

	MeshEntity * loaded1;

	Transform * t;

	BaseComponentShader * baseShader;
	BaseComponentShader * voxelShader;

	VoxelJoint * voxelJoint;

	explicit MainScene(Game * _game);
	~MainScene();
	void update() override;
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	void unload() override;
	void load() override;
};