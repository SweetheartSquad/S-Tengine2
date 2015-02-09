#pragma once

#include "PuppetScene.h"

#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"
#include "Keyboard.h"
#include "SoundManager.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "ControllableOrthographicCamera.h"
#include "RenderOptions.h"

#include <array>
#include <Box2D/Box2D.h>
#include <Box2DDebugDraw.h>
#include "Box2DMeshEntity.h"
#include "MeshFactory.h"
#include "PerspectiveCamera.h"
#include "MousePerspectiveCamera.h"
#include "BitmapFont.h"
#include "TestCharacter.h"
#include "CharacterComponent.h"
#include "GameJamContactListener.h"
#include "Game.h"
#include <Arduino.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>

#include <PuppetCharacter.h>
#include <PuppetController.h>

PuppetScene::PuppetScene(Game * _game):
	Scene(_game),
	cl(new GameJamContactListener),
	debugDraw(true),
	drawer(new Box2DDebugDraw(this, world)),
	world(new Box2DWorld(b2Vec2(0, -9.8))),
	playerCharacter(new PuppetCharacter(world, PLAYER, PROP | NPC, false)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager())
{
	world->b2world->SetContactListener(cl);
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	renderOptions->alphaSorting = true;
	ground->setShader(shader, true);
	ground->setTranslationPhysical(0, -5, -5.f);
	ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	ground->transform->scale(1000, 10, 1);
	ground->mesh->setUV(3, 0, 0);
	ground->mesh->setUV(2, 200.f, 0);
	ground->mesh->setUV(1, 200.f, 2.f);
	ground->mesh->setUV(0, 0, 2.f);
	ground->mesh->dirty = true;
	ground->mesh->uvEdgeMode = GL_REPEAT;
	ground->body->SetTransform(b2Vec2(0, -10), 0);
	
	ground->mesh->vertices.at(0).z -= 10;
	ground->mesh->vertices.at(1).z -= 10;
	ground->mesh->vertices.at(2).z -= 10;
	ground->mesh->vertices.at(3).z -= 10;

	world->addToWorld(ground, 2);
	
	addChild(ground);
//	camera = new PerspectiveCamera(playerCharacter->components.at(0), glm::vec3(0, 0, 0), 0, 0);
	camera = new MousePerspectiveCamera();
	camera->farClip = 1000.f;
	camera->transform->rotate(90, 0, 1, 0, kWORLD);
	camera->transform->translate(5.0f, 0.f, 15.0f);
	camera->yaw = 90.0f;
	camera->pitch = -10.0f;

	world->b2world->SetDebugDraw(drawer);
	//drawer->AppendFlags(b2Draw::e_aabbBit);
	drawer->AppendFlags(b2Draw::e_shapeBit);
	drawer->AppendFlags(b2Draw::e_centerOfMassBit);
	drawer->AppendFlags(b2Draw::e_jointBit);
	addChild(drawer);


	playerCharacter->setShader(shader, true);
	addChild(playerCharacter);
	playerCharacter->addToScene(this);
	playerCharacter->head->maxVelocity = b2Vec2(10, 10);
	
	TestCharacter * michael = new TestCharacter(world, false, PLAYER, PROP | NPC);
	michael->setShader(shader, true);
	addChild(michael);
	michael->addToScene(this);
	

	//Arduino 
	arduino = new AccelerometerParser("COM3");
	Accelerometer * acc = new Accelerometer(arduino);
	arduino->addAccelerometer(acc);
	//arduino->addAccelerometer(new Accelerometer(arduino));
	//arduino->addAccelerometer(new Accelerometer(arduino));
	//arduino->addAccelerometer(new Accelerometer(arduino));
	
	puppetController = new PuppetController(acc, playerCharacter);

}

PuppetScene::~PuppetScene(){
}

void PuppetScene::load(){
	Scene::load();
}

void PuppetScene::unload(){
	Scene::unload();
}

void PuppetScene::update(Step * _step){
	
	Scene::update(_step);
	world->update(_step);

	arduino->update(_step);
	puppetController->update(_step);

	if(keyboard->keyJustDown(GLFW_KEY_W)){
		if(playerCharacter->head->body->GetPosition().y < 12){
			playerCharacter->head->applyLinearImpulseUp(2500);
		}
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		//playerCharacter->transform->rotate(1, 0, 1, 0, kOBJECT);
		//playerCharacter->reactiveFeet = false;
		//playerCharacter->reactiveBody = false;
	}else{
		//playerCharacter->reactiveFeet = true;
		//playerCharacter->reactiveBody = true;
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter->head->applyLinearImpulseLeft(25);
		if(playerCharacter->transform->scaleVector.x < 0){
			playerCharacter->transform->scaleX(-1);
		}
		//playerCharacter->playAnimation = true;
		//playerCharacter->setCurrentAnimation("run");

		/*if(playerCharacter->reactiveFeet){
			b2ContactEdge * stuff = playerCharacter->leftLowerLeg->body->GetContactList();
			if(stuff != nullptr && stuff->contact->IsTouching()){
				int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
				int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

				if(type1 != type2){
					if(playerCharacter->leftLowerLeg->body->GetLinearVelocity().y <= 0.1){
						playerCharacter->leftLowerLeg->applyLinearImpulseLeft(5);
						playerCharacter->leftLowerLeg->applyLinearImpulseUp(125);	
						playerCharacter->torso->applyLinearImpulseUp(125);	
					}
				}
			}
			stuff = playerCharacter->rightLowerLeg->body->GetContactList();
			if(stuff != nullptr && stuff->contact->IsTouching()){
				int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
				int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

				if(type1 != type2){
					if(playerCharacter->rightLowerLeg->body->GetLinearVelocity().y <= 0.1){
						playerCharacter->rightLowerLeg->applyLinearImpulseLeft(5);
						playerCharacter->rightLowerLeg->applyLinearImpulseUp(125);
						playerCharacter->torso->applyLinearImpulseUp(125);	
					}
				}
			}
		}*/

	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter->head->applyLinearImpulseRight(25);
		if(playerCharacter->transform->scaleVector.x > 0){
			playerCharacter->transform->scaleX(-1);
		}
		//playerCharacter->setCurrentAnimation("run");
		//playerCharacter->playAnimation = true;

		/*if(playerCharacter->reactiveFeet){
			b2ContactEdge * stuff = playerCharacter->leftLowerLeg->body->GetContactList();
			if(stuff != nullptr && stuff->contact->IsTouching()){
				int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
				int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

				if(type1 != type2){
					if(playerCharacter->leftLowerLeg->body->GetLinearVelocity().y <= 0.1){
						playerCharacter->leftLowerLeg->applyLinearImpulseRight(5);
						playerCharacter->leftLowerLeg->applyLinearImpulseUp(50);	
						playerCharacter->torso->applyLinearImpulseUp(100);	
					}
				}
			}
			stuff = playerCharacter->rightLowerLeg->body->GetContactList();
			if(stuff != nullptr && stuff->contact->IsTouching()){
				int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
				int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

				if(type1 != type2){
					if(playerCharacter->rightLowerLeg->body->GetLinearVelocity().y <= 0.1){
						playerCharacter->rightLowerLeg->applyLinearImpulseRight(5);
						playerCharacter->rightLowerLeg->applyLinearImpulseUp(50);
						playerCharacter->torso->applyLinearImpulseUp(100);	
					}
				}
			}
		}*/
	}

	// move the ground and background with the player
	//ground->setTranslationPhysical(playerCharacter->transform->translationVector.x, ground->transform->translationVector.y, ground->transform->translationVector.z);

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
		game->toggleFullScreen();
	}
	if(keyboard->keyJustUp(GLFW_KEY_F1)){
		debugDraw = !debugDraw;
	}

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		debugDraw = !debugDraw;
	}
}

void PuppetScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
}