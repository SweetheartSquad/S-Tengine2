#pragma once

#include <Scene.h>

#include <MeshEntity.h>
#include <Texture.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentAlpha.h>
#include <OpenALSound.h>
#include <OrthographicCamera.h>
#include <Timeout.h>

class Scene_Splash : public Scene{
protected:
	glm::vec3 clearColour;
	MeshEntity * splash;
	ComponentShaderBase * shader;
	ShaderComponentAlpha * alphaComponent;
	OrthographicCamera * orthoCam;

	Timeout * timer;
	float aspectRatio;
public:
	std::string nextScene;

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	/*
	_splashImage: image displayed during splash
	_splashSound: sound played during splash
	_length: length of splash scene (in seconds, fade time is a tenth of this)
	_clearColour: clear colour of the scene (alpha is used to fade, so it will fade to and from this colour)
	*/
	Scene_Splash(Game * _game, Texture * _splashImage, OpenAL_Sound * _splashSound, float _length = 3.f, glm::vec3 _clearColour = glm::vec3(0.f));
	~Scene_Splash();
};