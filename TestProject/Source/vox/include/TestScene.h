#pragma once

#include <Scene.h>

class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDraw;
class Box2DMeshEntity;


class TestScene : public Scene{
public:
	BaseComponentShader * shader;
	MousePerspectiveCamera * mouseCam;
	FollowCamera * gameCam;

	Box2DWorld * world;
	Box2DDebugDraw * drawer;
	Box2DMeshEntity * player;

	void update(Step * _step) override;
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	TestScene(Game * _game);
	~TestScene();
};