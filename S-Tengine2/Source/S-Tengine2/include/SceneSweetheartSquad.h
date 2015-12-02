#pragma once

#include <Scene.h>
#include <SoundManager.h>
#include <UILayer.h>

class MeshEntity;

class Shader;
class ShaderComponentHsv;
class RenderSurface;
class StandardFrameBuffer;
class Sprite;
class Label;
class BulletWorld;
class BulletMeshEntity;


class SceneSweetheartSquad : public Scene{
public:
	ComponentShaderBase * diffuseShader;
	ComponentShaderBase * phongShader;
	ShaderComponentHsv * hsvComponent;
	
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	std::string nextScene;

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	UILayer uiLayer;
	Entity * logo;
	MeshEntity * logo1;
	MeshEntity * logo2;
	Label * textThing;
	float t;

	SceneSweetheartSquad(Game * _game);
	~SceneSweetheartSquad();
};