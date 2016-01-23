#pragma once

#include <Scene_Splash_Engine.h>

#include <MeshInterface.h>
#include <Easing.h>

#include <NumberUtils.h>
#include <scenario/Scenario.h>

Scene_Splash_Engine::Scene_Splash_Engine(Game * _game) :
	Scene_Splash(_game, new Texture("assets/engine basics/splash-engine.png", false, true), Scenario::defaultAudio->sound, 10.f)
{
	
	timer->eventManager->listeners["progress"].pop_back(); // remove the default transition effect

	timer->eventManager->addEventListener("progress", [this](sweet::Event * _event){
		float t = _event->getFloatData("progress");
		float b;
		if(t < 0.4f){
			b = Easing::easeInQuint(std::max(0.f, t), 0, 1, 0.4f) - std::rand() % 100 / 1000.f;
		}else{
			b = Easing::easeOutQuad(std::max(0.f, t - 0.925f), 1, -1, 0.1f) - std::rand() % 100 / 1000.f;
		}

		float thing = std::rand() % 50 / 10000.f;

		float v;
		if (t <= 0.7f){
			v = thing + Easing::easeOutCubic(std::min(0.6f, t), 0, -50, 0.6f);
		}else{
			v = thing + Easing::easeInQuint(std::max(0.f, t - 0.8f), 0, -25, 0.25f);
		}
		splash->mesh->vertices.at(0).v = v;
		splash->mesh->vertices.at(1).v = v;
		splash->mesh->vertices.at(2).v = v + 1;
		splash->mesh->vertices.at(3).v = v + 1;
		splash->mesh->dirty = true;
		alphaComponent->setAlpha(b);
	});
}

Scene_Splash_Engine::~Scene_Splash_Engine(){
}