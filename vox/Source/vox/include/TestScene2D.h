#pragma once

#include "Scene2D.h"

class Texture;
class Sprite;

class TestScene2D : public Scene2D
{
public:

	Sprite * sprite;
	Texture * tex;
	BaseComponentShader* shader;

	explicit TestScene2D(Game * _game);
	virtual ~TestScene2D();

	void load() override;
	void unload() override;
	void update() override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
};