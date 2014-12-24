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

SpriteSheetAnimation * an;
SpriteSheet * spriteSheet;
TestScene2D::TestScene2D(Game* _game)
	:Scene2D(_game),
	sprite(new Sprite(nullptr, new Transform())),
	tex(new Texture("../assets/spritesheet.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader())
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	sprite->setShader(shader, true);
	sprite->mesh->pushTexture2D(tex);
	addChild(sprite);	

	spriteSheet = new SpriteSheet(tex);
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

	an = new SpriteSheetAnimation(spriteSheet, 0.05);

	sprite->setUvs(spriteSheet->frames.at(0));
	sprite->transform->scale(-2, 2, 1);

	sprite->mesh->dirty = true;
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

	an->update(&vox::step);
	sprite->setUvs(spriteSheet->frames.at(an->currentFrame));
	sprite->mesh->dirty = true;
}

void TestScene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene2D::render(_matrixStack, _renderStack);
}