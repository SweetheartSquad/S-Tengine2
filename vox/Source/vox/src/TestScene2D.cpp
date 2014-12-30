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

#include <array>
#include <libzplay.h>
#include <Box2D/Box2D.h>

int velocityIterations = 6;
int positionIterations = 2;
b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);
b2Body* body;

SpriteSheetAnimation * spriteSheet;
TestScene2D::TestScene2D(Game* _game)
	:Scene2D(_game),
	sprite(new Sprite(nullptr, new Transform())),
	tex(new Texture("../assets/spritesheet.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager())
{
	soundManager->addNewSound("green_chair", "../assets/test.wav");
	soundManager->play("green_chair");
	
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();

	sprite->setShader(shader, true);
	addChild(sprite);	

	spriteSheet = new SpriteSheetAnimation(tex, 0.1);
	spriteSheet->pushFramesInRange(0, 26, 130, 150, 130 * 7);

	sprite->transform->scale(3, 3, 1);
	sprite->addAnimation("run", spriteSheet, true);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 10.0f);
    body = world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

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

	 world.Step(vox::step.getDeltaTime(), velocityIterations, positionIterations);
	
	 sprite->transform->translationVector.x = body->GetPosition().x;
	 sprite->transform->translationVector.y = body->GetPosition().y;

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