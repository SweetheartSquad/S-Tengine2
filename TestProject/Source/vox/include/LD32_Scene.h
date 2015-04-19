#pragma once

#include <Scene.h>
#include <SoundManager.h>


class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDraw;
class Box2DMeshEntity;
class MeshEntity;

class Shader;
class RenderSurface;
class StandardFrameBuffer;

#define numFFTsamples (256)
#define numHarmonics (numFFTsamples / 2 + 1)

class LD32_Scene : public Scene{
public:
	BaseComponentShader * shader;
	MousePerspectiveCamera * mouseCam;
	MousePerspectiveCamera * debugCam;
	FollowCamera * gameCam;

	Box2DWorld * world;
	Box2DDebugDraw * drawer;
	Box2DMeshEntity * player;
	
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;

	float sceneHeight;
	float sceneWidth;

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	std::vector<MeshEntity *> audioVisualizer;

	LD32_Scene(Game * _game);
	~LD32_Scene();
};