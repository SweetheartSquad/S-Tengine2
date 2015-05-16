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


class SceneSplash : public Scene{
public:
	BaseComponentShader * shader;
	ShaderComponentHsv * hsvComponent;
	
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	std::string nextScene;

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	UILayer uiLayer;
	Sprite * mouseIndicator;
	MeshEntity * logo;

	SceneSplash(Game * _game);
	~SceneSplash();
};