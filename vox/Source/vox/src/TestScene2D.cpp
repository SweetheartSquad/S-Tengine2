#pragma once

#include "TestScene2D.h"
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

#include <array>
#include <libzplay.h>
#include <Box2D/Box2D.h>
#include <Box2DDebugDraw.h>
#include "Box2DMeshEntity.h"
#include "MeshFactory.h"
#include "PerspectiveCamera.h"

TestScene2D::TestScene2D(Game * _game):
	Scene(_game),
	sprite(new Box2DSprite(b2_dynamicBody, true, nullptr, new Transform())),
	ground(new Box2DMeshEntity(MeshFactory::getCubeMesh(), b2_staticBody)),
	world(new Box2DWorld(b2Vec2(0, -60))),
	tex(new Texture("../assets/spritesheet.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager())
{
	Box2DDebugDraw * drawer = new Box2DDebugDraw(this);

	//camera = new ControllableOrthographicCamera(10, -10, -10, 10, 20, -20);
	//static_cast<ControllableOrthographicCamera*>(camera)->follow(sprite);

	soundManager->addNewSound("green_chair", "../assets/test.wav");
	soundManager->play("green_chair");
	
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	sprite->setShader(shader, true);
	ground->setShader(shader, true);
	
	addChild(sprite);	
	addChild(ground);

	spriteSheet = new SpriteSheetAnimation(tex, 0.05);
	spriteSheet->pushFramesInRange(0, 26, 130, 150, 130 * 7);
	sprite->addAnimation("run", spriteSheet, true);

	ground->setTranslationPhysical(0, -10, 0);
	ground->transform->scale(20, 10, 2);
	ground->mesh->pushTexture2D(new Texture("../assets/uv-test.jpg", 1000, 1000, true, true));

	sprite->transform->scale(2, 2, 1);
	sprite->setTranslationPhysical(0, 10, 0);

	sprite->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);

	world->addToWorld(sprite);
	world->addToWorld(ground);

	world->world->SetDebugDraw(drawer);
	drawer->SetFlags(b2Draw::e_shapeBit);
}

TestScene2D::~TestScene2D(){
}

void TestScene2D::load(){
	Scene::load();
}

void TestScene2D::unload(){
	Scene::unload();
}

void TestScene2D::update(Step * _step){

	Scene::update(_step);

	world->update(_step);
	sprite->playAnimation = false;

	if(keyboard->keyDown(GLFW_KEY_W)){
		if(!sprite->movingVertically(0.05)){
			sprite->applyLinearImpulseUp(500);	
		}
	}
	if(keyboard->keyDown(GLFW_KEY_S)){

	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		sprite->applyLinearImpulseLeft(50);
		if(sprite->transform->scaleVector.x > 0){
			sprite->transform->scaleX(-1);
		}
		sprite->playAnimation = true;
		sprite->setCurrentAnimation("run");
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		sprite->applyLinearImpulseRight(50);
		if(sprite->transform->scaleVector.x < 0){
			sprite->transform->scaleX(-1);
		}
		sprite->setCurrentAnimation("run");
		sprite->playAnimation = true;
	}

	//Add movement to the camera
	if(keyboard->keyDown(GLFW_KEY_W)){
		camera->transform->translate((camera->forwardVectorRotated) * static_cast<PerspectiveCamera *>(camera)->speed);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		camera->transform->translate((camera->forwardVectorRotated) * -static_cast<PerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		camera->transform->translate((camera->rightVectorRotated) * -static_cast<PerspectiveCamera *>(camera)->speed);		
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		camera->transform->translate((camera->rightVectorRotated) * static_cast<PerspectiveCamera *>(camera)->speed);	
	}
}

void TestScene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
	world->world->DrawDebugData();
}