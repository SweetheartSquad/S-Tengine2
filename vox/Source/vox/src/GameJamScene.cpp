#pragma once

#include "Arduino.h"
#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"
#include "shader/ShadowShaderComponent.h"
#include "shader/PhongShaderComponent.h"
#include "Keyboard.h"
#include "SpriteSheet.h"
#include "Rectangle.h"
#include "SpriteSheetAnimation.h"
#include "Vox.h"
#include "SoundManager.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "ControllableOrthographicCamera.h"
#include "Resource.h"
#include "RenderOptions.h"

#include <array>
#include <libzplay.h>
#include <Box2D/Box2D.h>
#include <Box2DDebugDraw.h>
#include "Box2DMeshEntity.h"
#include "MeshFactory.h"
#include "PerspectiveCamera.h"
#include "MousePerspectiveCamera.h"
#include "GameJamScene.h"
#include "BitmapFont.h"
#include "CylinderScreen.h"

GameJamScene::GameJamScene(Game * _game):
	Scene(_game),
	world(new Box2DWorld(b2Vec2(0, -60))),
	playerSprite(new Box2DSprite(world, b2_dynamicBody, true, nullptr, new Transform())),
	ground(new Box2DMeshEntity(world, MeshFactory::getCubeMesh(), b2_staticBody)),
	tex(new Texture("../assets/MichaelScale.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	backgroundScreen(new CylinderScreen(25, &playerSprite->transform->translationVector.x, 4, new Texture("../assets/sky.png", 4096, 4096, true, true))),
	midgroundScreen(new CylinderScreen(50, &playerSprite->transform->translationVector.x, 4, new Texture("../assets/sky2.png", 4096, 4096, true, true))),
	foregroundScreen(new CylinderScreen(75, &playerSprite->transform->translationVector.x, 4, new Texture("../assets/sky3.png", 4096, 4096, true, true))),
	drawer(new Box2DDebugDraw(this))
{
	renderOptions->alphaSorting = true;
	camera->transform->rotate(90, 0, 1, 0, kWORLD);

	soundManager->addNewSound("green_chair", "../assets/test.wav");
	//soundManager->play("green_chair");
	
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	playerSprite->mesh->pushTexture2D(tex);
	playerSprite->setShader(shader, true);
	ground->setShader(shader, true);

	ground->setTranslationPhysical(0, -10, -3);
	ground->transform->scale(50, 10, 2);
	ground->mesh->pushTexture2D(new Texture("../assets/uv-test.jpg", 1000, 1000, true, true));

	playerSprite->transform->scale(4, 4, 1);
	playerSprite->setTranslationPhysical(0, 10, 0);

	playerSprite->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);

	world->addToWorld(playerSprite);
	world->addToWorld(ground);

	
	backgroundScreen->transform->rotate(-90.f, 0.f, 1.f, 0.f, CoordinateSpace::kOBJECT);
	backgroundScreen->transform->scale(25, 75, 75);
	backgroundScreen->transform->translate(0, -10, -20);
	backgroundScreen->setShader(shader, true);
	
	midgroundScreen->transform->rotate(-90.f, 0.f, 1.f, 0.f, CoordinateSpace::kOBJECT);
	midgroundScreen->transform->scale(15, 50, 50);
	midgroundScreen->transform->translate(0, -10, -10);
	midgroundScreen->setShader(shader, true);
	
	foregroundScreen->transform->rotate(-90.f, 0.f, 1.f, 0.f, CoordinateSpace::kOBJECT);
	foregroundScreen->transform->scale(-5, 25, 25);
	foregroundScreen->transform->translate(0, -10, 10);
	foregroundScreen->setShader(shader, true);
	
	
	Texture * font = new Texture("../assets/MoonFlowerBold.png", 1024, 1024, true, true);
	BitmapFont * fontM = new BitmapFont(font, 32, 16, 16); 
    fontM->setText("sdsdweqweqwewqesdsdsdadasd");
	fontM->transform->translate(0, 3, 5);
	fontM->setShader(shader, true);

	
	addChild(midgroundScreen);
	addChild(playerSprite);
	addChild(ground);
	addChild(foregroundScreen);
	addChild(fontM);
	addChild(backgroundScreen);
	
	/*addChild(foregroundScreen);
	addChild(fontM);
	addChild(playerSprite);
	addChild(ground);
	addChild(midgroundScreen);
	addChild(backgroundScreen);*/
	std::cout << "add order: " << std::endl;
	std::cout << foregroundScreen->transform->translationVector.z << std::endl;
	std::cout << fontM->transform->translationVector.z << std::endl;
	std::cout << playerSprite->transform->translationVector.z << std::endl;
	std::cout << ground->transform->translationVector.z << std::endl;
	std::cout << midgroundScreen->transform->translationVector.z << std::endl;
	std::cout << backgroundScreen->transform->translationVector.z << std::endl << std::endl << "final order:" << std::endl;
	for(unsigned long int i = 0; i < children.size(); ++i){
		std::cout << children.at(i)->transform->translationVector.z << std::endl;
	}

	
	camera = new PerspectiveCamera(playerSprite, glm::vec3(0, 10, 0), 5, 0);
	//camera = new MousePerspectiveCamera();
	camera->transform->translate(5.0f, 5.0f, 20.0f);
	camera->yaw = 90.0f;
	camera->pitch = -10.0f;

	world->b2world->SetDebugDraw(drawer);
	drawer->SetFlags(b2Draw::e_shapeBit);
}

GameJamScene::~GameJamScene(){
}

void GameJamScene::load(){
	Scene::load();
	drawer->load();
}

void GameJamScene::unload(){
	Scene::unload();
	drawer->unload();
}

void GameJamScene::update(Step * _step){
	Scene::update(_step);

	world->update(_step);
	playerSprite->playAnimation = false;
	if(keyboard->keyDown(GLFW_KEY_W)){
		if(!playerSprite->movingVertically(0.05)){
			playerSprite->applyLinearImpulseUp(500);	
		}
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		playerSprite->transform->rotate(1, 0, 1, 0, kOBJECT);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		playerSprite->applyLinearImpulseLeft(50);
		if(playerSprite->transform->scaleVector.x > 0){
			playerSprite->transform->scaleX(-1);
		}
		//playerSprite->playAnimation = true;
		//playerSprite->setCurrentAnimation("run");
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerSprite->applyLinearImpulseRight(50);
		if(playerSprite->transform->scaleVector.x < 0){
			playerSprite->transform->scaleX(-1);
		}
		//playerSprite->setCurrentAnimation("run");
		//playerSprite->playAnimation = true;
	}

	// move the ground and background with the player
	ground->setTranslationPhysical(playerSprite->transform->translationVector.x, ground->transform->translationVector.y, ground->transform->translationVector.z);


	// camera controls
	if(keyboard->keyDown(GLFW_KEY_UP)){
		camera->transform->translate((camera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);
	}
	if(keyboard->keyDown(GLFW_KEY_DOWN)){
		camera->transform->translate((camera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyDown(GLFW_KEY_LEFT)){
		camera->transform->translate((camera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(camera)->speed);		
	}
	if(keyboard->keyDown(GLFW_KEY_RIGHT)){
		camera->transform->translate((camera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		Scene::toggleFullScreen();
	}
}

void GameJamScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
	world->b2world->DrawDebugData();
}
