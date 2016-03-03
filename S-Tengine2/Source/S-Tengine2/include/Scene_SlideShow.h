#pragma once

#include <Scene.h>

#include <Texture.h>
#include <OpenALSound.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentAlpha.h>
#include <Easing.h>

class Slide : public Node{
public:
	Texture * tex;
	OpenAL_Sound * sound;

	Slide(Texture * _tex, float _length = 1.f, OpenAL_Sound * _sound = nullptr);
	~Slide();
};

namespace sweet{
	class EventManager;
};

class Timeout;
class NodeUI;
class UILayer;

class Scene_SlideShow : public Scene{
private:
	Slide * currSlide;
	std::vector<Slide *> forwards;
	std::vector<Slide *> backwards;

	NodeUI * slide;
	NodeUI * slideOld;
	NodeUI * slideNew;

	bool isTransitioning;
	bool isTransitioningForwards;

	void next();
	void prev();
	void setNewSlide(bool _isForwards);
public:
	Scene_SlideShow(Game * _game, Easing::Type _easeType = Easing::kEASE_IN_OUT_CUBIC);
	~Scene_SlideShow();
	
	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	Timeout * transition;
	sweet::EventManager * eventManager;

	void push(Slide * _slide);
	
	void changeSlide(bool _isForwards);

	UILayer * uiLayer;
};