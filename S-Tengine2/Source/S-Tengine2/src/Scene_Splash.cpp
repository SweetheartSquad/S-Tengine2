#pragma once

#include <Scene_Splash.h>

#include <shader/ShaderComponentMVP.h>
#include <shader/ShaderComponentTexture.h>

#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Keyboard.h>
#include <Game.h>
#include <RenderOptions.h>

Scene_Splash::Scene_Splash(Game * _game, Texture * _splashImage, OpenAL_Sound * _splashSound, float _length, glm::vec3 _clearColour) :
	Scene(_game),
	clearColour(_clearColour),
	splash(nullptr),
	shader(new ComponentShaderBase(true)),
	alphaComponent(new ShaderComponentAlpha(shader, 0)),
	nextScene(""),
	aspectRatio((float)_splashImage->width/_splashImage->height)
{
	shader->addComponent(new ShaderComponentMVP(shader));
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->addComponent(alphaComponent);
	shader->compileShader();

	orthoCam = new OrthographicCamera(0, 0, 0, 0, -1000, 1000);
	cameras.push_back(orthoCam);
	activeCamera = orthoCam;

	//Set up cameras
	/*PerspectiveCamera * fc = new PerspectiveCamera();
	childTransform->addChild(fc);
	cameras.push_back(fc);
	fc->farClip = 1000.f;
	fc->nearClip = 0.001f;
	fc->childTransform->rotate(90, 0, 1, 0, kWORLD);
	fc->parents.at(0)->translate(-18.5f, -2.0f, 17.36f);
	fc->yaw = 60.0f;
	fc->pitch = -4.0f;
	fc->lastOrientation = fc->calcOrientation();
	fc->childTransform->setOrientation(fc->lastOrientation);
	activeCamera = fc;*/
	
	splash = new MeshEntity(MeshFactory::getPlaneMesh(), shader);
	splash->mesh->pushTexture2D(_splashImage);
	childTransform->addChild(splash);
	for(unsigned long int i = 0; i < splash->mesh->vertices.size(); ++i){
		splash->mesh->vertices.at(i).y += 0.5f;
	}
	splash->mesh->dirty = true;


	
	timer = new Timeout(_length, [this](sweet::Event * _event){
		game->switchScene(nextScene, true);
	});
	timer->eventManager->addEventListener("progress", [this](sweet::Event * _event){
		float p = _event->getFloatData("progress");
		if(p <= 0.1f){
			alphaComponent->setAlpha(p*10.f);
		}else if(p >= 0.9f){
			alphaComponent->setAlpha((1.f - p)*10.f);
		}else{
			alphaComponent->setAlpha(1.f);
		}
	});
	if(_splashSound != nullptr){
		timer->eventManager->addEventListener("start", [_splashSound](sweet::Event * _event){
			_splashSound->play();
		});
	}

	++shader->referenceCount;
}

Scene_Splash::~Scene_Splash(){
	shader->decrementAndDelete();
	delete timer;
}

void Scene_Splash::update(Step * _step){
	timer->start();
	timer->update(_step);
	glm::uvec2 sd = sweet::getWindowDimensions();

	orthoCam->resize(0, sd.x, 0, sd.y);

	if(splash != nullptr){
		float s = std::min(sd.x, (sd.y));
		splash->childTransform->scale(s*aspectRatio, s, 1, false);
		splash->childTransform->translate(sd.x/2.f,0,0,false);
	}

	if(Keyboard::getInstance().keyJustUp(GLFW_KEY_ENTER)){
		timer->eventManager->triggerEvent("complete");
	}
	
	Scene::update(_step);
}

void Scene_Splash::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}
	_renderOptions->setClearColour(clearColour.r, clearColour.g, clearColour.b, 1.f);
	_renderOptions->clear();
	Scene::render(_matrixStack, _renderOptions);
}

void Scene_Splash::load(){
	if(!loaded){
		shader->load();
	}
	Scene::load();	
}

void Scene_Splash::unload(){
	if(loaded){
		shader->unload();
	}
	Scene::unload();
}