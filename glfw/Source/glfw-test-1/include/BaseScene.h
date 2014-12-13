#pragma once

#include "Scene.h"

class Cube;
class BaseComponentShader;

class BaseScene : public Scene
{
public:
	Cube * cube;
	BaseComponentShader * shader;

	explicit BaseScene(Game* _game);
	virtual ~BaseScene();

	virtual void update() override;
	virtual void render() override;
};