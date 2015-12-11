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


class SceneSplash : public Scene{
public:
	ComponentShaderBase * shader;
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
	MeshEntity * logo;
	Label * textThing;
	float t;

	SceneSplash(Game * _game);
	~SceneSplash();
};