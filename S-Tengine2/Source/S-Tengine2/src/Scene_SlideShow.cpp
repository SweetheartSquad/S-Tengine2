#pragma once

#include <Scene_SlideShow.h>
#include <UILayer.h>
#include <MeshInterface.h>
#include <EventManager.h>
#include <Keyboard.h>
#include <shader/ShaderComponentMVP.h>
#include <shader/ShaderComponentTexture.h>
#include <Timeout.h>

Slide::Slide(Texture * _tex, float _length, OpenAL_Sound * _sound) :
	tex(_tex),
	sound(_sound)
{
	++tex->referenceCount;
	if(sound != nullptr){
		++sound->referenceCount;
	}
}

Slide::~Slide(){
	tex->decrementAndDelete();
	if(sound != nullptr){
		sound->decrementAndDelete();
	}
}

Scene_SlideShow::Scene_SlideShow(Game * _game, Easing::Type _easeType) :
	Scene(_game),
	currSlide(nullptr),
	isTransitioning(false),
	isTransitioningForwards(true),
	slide(nullptr)
{
	uiLayer = new UILayer(0,0,0,0);
	eventManager = new sweet::EventManager();

	slideOld = new NodeUI(uiLayer->world);
	slideOld->setRationalWidth(1.f, uiLayer);
	slideOld->setRationalHeight(1.f, uiLayer);

	slideNew= new NodeUI(uiLayer->world);
	slideNew->setRationalWidth(1.f, uiLayer);
	slideNew->setRationalHeight(1.f, uiLayer);

	uiLayer->addChild(slideOld);
	uiLayer->addChild(slideNew);
	
	uiLayer->addMouseIndicator();

	// initialize transition
	transition = new Timeout(1.f, [this](sweet::Event * _event){
		eventManager->triggerEvent("transitionComplete");
	});

	// add a default fade-in/out transition
	transition->eventManager->addEventListener("progress", [this, _easeType](sweet::Event * _event){
		float p = _event->getFloatData("progress");
		p = Easing::call(_easeType, p, 0, 1, 1);
		if(isTransitioningForwards){
			// right to left
			slideOld->childTransform->translate(-slideOld->getWidth() * p, 0, 0, false);
			slideNew->childTransform->translate(slideNew->getWidth() * (1 - p), 0, 0, false);
		}else{
			// left to right
			slideOld->childTransform->translate(slideOld->getWidth() * p, 0, 0, false);
			slideNew->childTransform->translate(-slideNew->getWidth() * (1 - p), 0, 0, false);
		}
	});

	eventManager->addEventListener("transitionComplete", [this](sweet::Event * _event){
		isTransitioning = false;
		slide = slideNew;
		if(currSlide->sound != nullptr){
			currSlide->sound->play();
		}		
	});
	
}

Scene_SlideShow::~Scene_SlideShow(){
	delete uiLayer;
}

void Scene_SlideShow::update(Step * _step){
	glm::vec2 sd = sweet::getWindowDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	eventManager->update(_step);
	transition->update(_step);
	Scene::update(_step);
	uiLayer->update(_step);
}

void Scene_SlideShow::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Scene::render(_matrixStack, _renderOptions);
	uiLayer->render(_matrixStack, _renderOptions);
}

void Scene_SlideShow::push(Slide * _slide){
	forwards.push_back(_slide);
}

void Scene_SlideShow::next(){
	if(forwards.size() > 0){
		if(currSlide != nullptr){
			backwards.push_back(currSlide);
		}
	
		currSlide = forwards.front();
		forwards.erase(forwards.begin());
	}
}

void Scene_SlideShow::prev(){
	if(backwards.size() > 0){
		if(currSlide != nullptr){
			forwards.insert(forwards.begin(), currSlide);
		}
	
		currSlide = backwards.back();
		backwards.pop_back();
	}
}

void Scene_SlideShow::changeSlide(bool _isForwards){
	if(_isForwards){
		if(forwards.size() > 0){
			setNewSlide(true);
		}else{
			eventManager->triggerEvent("overflow");
		}
	}else{
		if(backwards.size() > 0){
			setNewSlide(false);
		}else{
			eventManager->triggerEvent("underflow");
		}
	}
}

void Scene_SlideShow::setNewSlide(bool _isForwards){
	if(!isTransitioning){
		isTransitioning = true;
		isTransitioningForwards = _isForwards;
		transition->restart();

		// setup transition
		if(slide != nullptr){
			// The fancy switcharoo
			slideNew = slideOld;
			slideOld = slide;
		}
		// get next slide
		if(isTransitioningForwards){
			next();
		}else{
			prev();
		}

		if(currSlide != nullptr){
			slideNew->background->mesh->replaceTextures(currSlide->tex);
			slideOld->childTransform->translate(0, 0, 0, false);
			if(isTransitioningForwards){
				slideNew->childTransform->translate(slideNew->getWidth(), 0, 0, false);
			}else{
				slideNew->childTransform->translate(-slideNew->getWidth(), 0, 0, false);
			}
		}else{
			// Can't transition
			Log::error("No slide could be found");
			isTransitioning = false;
		}
		/*
		slideNew->setBackgroundColour(1.f, 0, 0);
		slideOld->setBackgroundColour(0, 1.f, 0);
		*/
	}
}