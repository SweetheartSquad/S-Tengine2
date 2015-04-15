#pragma once

#include <Scene.h>

class MousePerspectiveCamera;
class FollowCamera;

class TestScene : public Scene{
public:
	BaseComponentShader * shader;
	MousePerspectiveCamera * mouseCam;
	FollowCamera * gameCam;

	void update(Step * _step) override;

	TestScene(Game * _game);
	~TestScene();
};