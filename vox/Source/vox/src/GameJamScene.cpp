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
#include "TestCharacter.h"
#include "CharacterComponent.h"
#include "Character1.h"
#include "Character2.h"
#include "Character3.h"
#include "Character4.h"

GameJamScene::GameJamScene(Game * _game):
	Scene(_game),
	world(new Box2DWorld(b2Vec2(0, -60))),
	playerCharacter(new Character4(world)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody)),
	tex(new Texture("../assets/MichaelScale.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	backgroundScreen(new CylinderScreen(75, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/skybox - HD.png", 4096, 4096, true, true))),
	midgroundScreen(new CylinderScreen(50, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/walls - HD.png", 4096, 4096, true, true))),
	foregroundScreen(new CylinderScreen(25, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/sky3.png", 512, 512, true, true))),
	drawer(new Box2DDebugDraw(this))
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	renderOptions->alphaSorting = true;
	{
		Box2DSprite * s = new Box2DSprite(world, b2_staticBody, false, nullptr, new Transform());
		s->mesh->pushTexture2D(new Texture("../assets/Table.png", 256, 256, true, true));
		s->transform->scale(5, 5, 1);

		b2CircleShape shape;
		s->body->CreateFixture(&shape, 1);
		s->body->GetFixtureList()->SetSensor(true);
		s->setShader(shader, true);
		s->setTranslationPhysical(3, 10, -3);
		items.push_back(s);
	}
	{	
		Box2DSprite * s = new Box2DSprite(world, b2_staticBody, false, nullptr, new Transform());
		s->mesh->pushTexture2D(new Texture("../assets/TourDePizza_738_854.png", 1024, 1024, true, true));
		s->transform->scale(5, 5, 1);

		b2CircleShape shape;
		s->body->CreateFixture(&shape, 1);
		s->body->GetFixtureList()->SetSensor(true);
		s->setShader(shader, true);
		s->setTranslationPhysical(12, 10, -3);
		items.push_back(s);
	}
	soundManager->addNewSound("green_chair", "../assets/test.wav");
	//soundManager->play("green_chair");

	
	ground->setShader(shader, true);
	ground->setTranslationPhysical(0, 0, -5.f);
	ground->transform->rotate(90.f, 1, 0, 0, kOBJECT);
	ground->transform->scale(1000, 10, 1);
	ground->mesh->pushTexture2D(new Texture("../assets/hallwaycarpet.png", 1024, 1024, true, true));
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
	for(Box2DSprite * s : items){
		world->addToWorld(s);
	}

	backgroundScreen->transform->rotate(-90.f, 0.f, 1.f, 0.f, CoordinateSpace::kOBJECT);
	backgroundScreen->transform->scale(25, 100, 100);
	backgroundScreen->transform->translate(0, 0, -50);
	backgroundScreen->setShader(shader, true);
	
	midgroundScreen->transform->rotate(-90.f, 0.f, 1.f, 0.f, CoordinateSpace::kOBJECT);
	midgroundScreen->transform->scale(25, 43, 43);
	midgroundScreen->transform->translate(0, -6, -19);
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
	
	addChild(ground);
	//addChild(foregroundScreen);
	addChild(fontM);
	addChild(backgroundScreen);
	for(Box2DSprite * s : items){
		addChild(s);
	}

	camera = new PerspectiveCamera(playerCharacter->torso, glm::vec3(0, 7.5, 0), 1, 0);
	//camera = new MousePerspectiveCamera();
	camera->farClip = 100000000.f;
	camera->transform->rotate(90, 0, 1, 0, kWORLD);
	camera->transform->translate(5.0f, 0.f, 15.0f);
	camera->yaw = 90.0f;
	camera->pitch = -10.0f;

	world->b2world->SetDebugDraw(drawer);
	drawer->SetFlags(b2Draw::e_shapeBit);

	playerCharacter->setShader(shader);
	addChild(playerCharacter);
	playerCharacter->addToScene(this);
	playerCharacter->torso->setTranslationPhysical(15, 5, 0);
	playerCharacter->head->setTranslationPhysical(15, 5, 0);
	playerCharacter->torso->maxVelocity = b2Vec2(10, NO_VELOCITY_LIMIT);
	playerCharacter->torso->body->SetGravityScale(0);
	playerCharacter->torso->body->SetGravityScale(0);
	//ch->transform->scale(5, 5, 1);


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
	if(keyboard->keyJustDown(GLFW_KEY_W)){
		//if(!playerCharacter->torso->movingVertically(0.05)){
			playerCharacter->torso->applyLinearImpulseUp(250);	
		//}
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		//playerCharacter->transform->rotate(1, 0, 1, 0, kOBJECT);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		playerCharacter->torso->applyLinearImpulseLeft(25);
		if(playerCharacter->transform->scaleVector.x < 0){
			playerCharacter->transform->scaleX(-1);
		}
		//playerCharacter->playAnimation = true;
		//playerCharacter->setCurrentAnimation("run");

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
			std::cout << type1 << " " << type2 << std::endl;
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
			std::cout << type1 << " " << type2 << std::endl;
		}



	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		playerCharacter->torso->applyLinearImpulseRight(25);
		if(playerCharacter->transform->scaleVector.x > 0){
			playerCharacter->transform->scaleX(-1);
		}
		//playerCharacter->setCurrentAnimation("run");
		//playerCharacter->playAnimation = true;


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
			std::cout << type1 << " " << type2 << std::endl;
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
			std::cout << type1 << " " << type2 << std::endl;
		}



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
		Scene::toggleFullScreen();
	}
}

void GameJamScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
	world->b2world->DrawDebugData();
}
