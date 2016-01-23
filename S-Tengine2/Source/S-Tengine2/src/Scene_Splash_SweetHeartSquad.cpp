#pragma once

#include <Scene_Splash_SweetHeartSquad.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <Easing.h>

#include <scenario/Scenario.h>

Scene_Splash_SweetHeartSquad::Scene_Splash_SweetHeartSquad(Game * _game) :
	Scene_Splash(_game, new Texture("assets/engine basics/splash-sweetheartsquad.png", false, true), new OpenAL_SoundSimple("assets/engine basics/SweetHeart Squad.ogg", false, true, "sfx"), 4.f)
{
	splash->mesh->uvEdgeMode = GL_CLAMP;

	timer->eventManager->addEventListener("progress", [this](sweet::Event * _event){
		float t = _event->getFloatData("progress");

		float v = Easing::easeOutElastic(std::max(0.f, t), -1, 1, 3.0f/4.f, 1.2f, -1.f, 1.02f);
		
		if(t > 0.5f){
			v += Easing::easeInBack(std::max(0.f, t - 0.5f), 0, -1, 0.5f, 0.75f);
		}

		splash->mesh->vertices.at(0).v = v;
		splash->mesh->vertices.at(1).v = v;
		splash->mesh->vertices.at(2).v = v + 1;
		splash->mesh->vertices.at(3).v = v + 1;
		splash->mesh->dirty = true;
	});
}

Scene_Splash_SweetHeartSquad::~Scene_Splash_SweetHeartSquad(){}