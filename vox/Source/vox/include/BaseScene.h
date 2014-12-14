#pragma once

#include "Scene.h"

class MeshInterface;
class MeshEntity;
class DirectionalLight;
class PointLight;
class Material;
class Plane;
class Cube;
class BaseComponentShader;

class BaseScene : public Scene{
public:

	//Entities
	Cube * ground;
	Cube * cube;
	MeshEntity * monkey;

	//Shaders
	BaseComponentShader * shader;

	//Materials
	Material * material;

	//Lights
	PointLight * pointLight;
	DirectionalLight * keyLight;

	//Meshes
	MeshInterface * monkeyMesh;

	explicit BaseScene(Game* _game);
	virtual ~BaseScene();

	virtual void update() override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
};