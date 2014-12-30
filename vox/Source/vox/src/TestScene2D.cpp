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

#include <array>
#include <libzplay.h>
#include <Box2D/Box2D.h>

TestScene2D::TestScene2D(Game* _game):
	Scene2D(_game),
	sprite(new Box2DSprite(b2_dynamicBody, true, nullptr, new Transform())),
	ground(new Box2DSprite(b2_staticBody, true, nullptr, new Transform())),
	world(new Box2DWorld(b2Vec2(0, -40))),
	tex(new Texture("../assets/spritesheet.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager())
{
	soundManager->addNewSound("green_chair", "../assets/test.wav");
	soundManager->play("green_chair");
	
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	sprite->setShader(shader, true);
	ground->setShader(shader, true);
	
	addChild(sprite);	
	addChild(ground);

	spriteSheet = new SpriteSheetAnimation(tex, 0.1);
	spriteSheet->pushFramesInRange(0, 26, 130, 150, 130 * 7);

	sprite->transform->scale(2, 2, 1);
	sprite->addAnimation("run", spriteSheet, true);

	ground->setTranslationPhysical(0, -10, 0);
	ground->transform->scale(20, 10, 0);

	sprite->setTranslationPhysical(0, 10, 0);

	world->addToWorld(sprite);
	world->addToWorld(ground);
}

TestScene2D::~TestScene2D(){
}

void TestScene2D::load(){
	Scene2D::load();
}

void TestScene2D::unload(){
	Scene2D::unload();
}

void TestScene2D::update(){

	 Scene2D::update();

	 world->update(&vox::step);
	
	if(keyboard->keyDown(GLFW_KEY_W)){
		sprite->applyForceUp(8000);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		sprite->applyForceDown(5);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		sprite->applyLinearImpulseLeft(5);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		sprite->applyForceRight(600);
	}
}

void TestScene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene2D::render(_matrixStack, _renderStack);
}