#pragma once

#include <Scene.h>

class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDraw;


class TestScene : public Scene{
public:
	BaseComponentShader * shader;
	MousePerspectiveCamera * mouseCam;
	FollowCamera * gameCam;

	Box2DWorld * world;
	Box2DDebugDraw * drawer;

	void update(Step * _step) override;

	TestScene(Game * _game);
	~TestScene();
};