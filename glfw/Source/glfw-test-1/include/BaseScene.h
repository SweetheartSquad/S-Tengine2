#pragma once

#include "Scene.h"

class PointLight;
class Material;
class Plane;
class Cube;
class BaseComponentShader;

class BaseScene : public Scene
{
public:

	//Entities
	Cube * ground;
	Cube * cube;

	//Shaders
	BaseComponentShader * shader;

	//Materials
	Material * material;

	//Lights
	PointLight * light; 

	explicit BaseScene(Game* _game);
	virtual ~BaseScene();

	virtual void update() override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
};