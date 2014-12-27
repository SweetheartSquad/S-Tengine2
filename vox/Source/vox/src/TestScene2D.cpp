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
#include <array>

SpriteSheetAnimation * spriteSheet;
TestScene2D::TestScene2D(Game* _game)
	:Scene2D(_game),
	sprite(new Sprite(nullptr, new Transform())),
	tex(new Texture("../assets/spritesheet.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader())
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	sprite->setShader(shader, true);
	//sprite->mesh->pushTexture2D(tex);
	addChild(sprite);	

	spriteSheet = new SpriteSheetAnimation(tex, 0.1);
/*
	spriteSheet->pushFrame(0, 0, 130, 150);
	spriteSheet->pushFrame(1, 0, 130, 150);
	spriteSheet->pushFrame(2, 0, 130, 150);
	spriteSheet->pushFrame(3, 0, 130, 150);
	spriteSheet->pushFrame(4, 0, 130, 150);
	spriteSheet->pushFrame(5, 0, 130, 150);
	spriteSheet->pushFrame(6, 0, 130, 150);

	spriteSheet->pushFrame(0, 1, 130, 150);
	spriteSheet->pushFrame(1, 1, 130, 150);
	spriteSheet->pushFrame(2, 1, 130, 150);
	spriteSheet->pushFrame(3, 1, 130, 150);
	spriteSheet->pushFrame(4, 1, 130, 150);
	spriteSheet->pushFrame(5, 1, 130, 150);
	spriteSheet->pushFrame(6, 1, 130, 150);

	spriteSheet->pushFrame(0, 2, 130, 150);
	spriteSheet->pushFrame(1, 2, 130, 150);
	spriteSheet->pushFrame(2, 2, 130, 150);
	spriteSheet->pushFrame(3, 2, 130, 150);
	spriteSheet->pushFrame(4, 2, 130, 150);
	spriteSheet->pushFrame(5, 2, 130, 150);
	spriteSheet->pushFrame(6, 2, 130, 150);

	spriteSheet->pushFrame(0, 3, 130, 150);
	spriteSheet->pushFrame(1, 3, 130, 150);
	spriteSheet->pushFrame(2, 3, 130, 150);
	spriteSheet->pushFrame(3, 3, 130, 150);
	spriteSheet->pushFrame(4, 3, 130, 150);
	spriteSheet->pushFrame(5, 3, 130, 150);*/

	int f[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
	std::vector<int> ff(std::begin(f), std::end(f));
	//spriteSheet->pushMultipleFrames(ff, 130, 150, 130 * 7);

	spriteSheet->pushFramesInRange(20, 21, 130, 150, 130 * 7);

	sprite->transform->scale(3, 3, 1);
	
	sprite->addAnimation("run", spriteSheet, true);
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

	if(keyboard->keyDown(GLFW_KEY_W)){
		sprite->transform->translate(0.f, 0.1f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		sprite->transform->translate(0.f, -0.1f, 0.f);	
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		sprite->transform->translate(0.1f, 0.f, 0.f);	
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		sprite->transform->translate(-0.1f, 0.f, 0.f);	
	}
}

void TestScene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene2D::render(_matrixStack, _renderStack);
}