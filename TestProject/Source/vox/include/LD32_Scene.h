#pragma once

#include <Scene.h>
#include <SoundManager.h>


class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDraw;
class Box2DMeshEntity;
class MeshEntity;

class ShaderComponentHsv;
class LD32_Monster;
class LD32_Enemy;

class Shader;
class RenderSurface;
class StandardFrameBuffer;
class Material;

#define numFFTsamples (32)
#define numHarmonics (numFFTsamples / 2 + 1)

class LD32_Player;

class LD32_Scene : public Scene{
public:
	BaseComponentShader * shader;
	ShaderComponentHsv * hsvComponent;
	MousePerspectiveCamera * mouseCam;
	MousePerspectiveCamera * debugCam;
	FollowCamera * gameCam;

	Box2DWorld * world;
	Box2DDebugDraw * drawer;
	LD32_Player * player;
	LD32_Monster * monster;
	std::vector<LD32_Enemy *> enemies;
	
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	Material * phongMat;

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