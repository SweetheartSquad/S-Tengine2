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
	playerCharacter(new TestCharacter(world, false, CHARACTER, BOUNDARY)),
	ground(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody)),
	tex(new Texture("../assets/MichaelScale.png", 1024, 1024, true, true)),
	shader(new BaseComponentShader()),
	soundManager(new SoundManager()),
	backgroundScreen(new CylinderScreen(75, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/skybox - HD.png", 4096, 4096, true, true))),
	midgroundScreen(new CylinderScreen(50, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/walls - HD.png", 4096, 4096, true, true))),
	foregroundScreen(new CylinderScreen(50, &playerCharacter->torso->transform->translationVector.x, 4, new Texture("../assets/foregroundhallway.png", 4096, 4096, true, true))),
	drawer(new Box2DDebugDraw(this))
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	renderOptions->alphaSorting = true;

	
	const int imgCount = 12;
	std::string strings[imgCount] = {
		"../assets/Table_657_332.png",		
		"../assets/TourDePizza_738_854.png",
		"../assets/ArtisticStatue_820_915.png",
		"../assets/BullitenBoard_573_353.png",
		"../assets/Cat_441_726.png",
		"../assets/Creepy_263_470.png",
		"../assets/DeadPlant_228_468.png",
		"../assets/Garbage_300_374.png",
		"../assets/HealthyPlant_272_877.png",
		"../assets/JollySkeleton_399_916.png",
		"../assets/Lockers_1024_604.png",
		"../assets/Pie_667_692.png"
	};

	for(unsigned long int i = 0; i < imgCount; ++i){
		Box2DSprite * s = new Box2DSprite(world, b2_staticBody, false, nullptr, new Transform());
		s->mesh->pushTexture2D(new Texture(strings[i].c_str(), 1024, 1024, true, true));

		int width = 0;
		int height = 0;

		int cc = 0;
		for(char c : strings[i]){
			if(c == '_' || c == '.'){
				strings[i].at(cc) = ' ';
			}
			cc++;
		}
		std::string arr[4];
		int j = 0;
		std::stringstream ssin(strings[i]);
		while (ssin.good() && i < 4){
			ssin >> arr[j];
			++j;
		}

		width =  atoi(arr[1].c_str());
		height =  atoi(arr[2].c_str());

		float scale = 0.002;

		b2PolygonShape tShape;
		tShape.SetAsBox(width*std::abs(s->transform->scaleVector.x)*scale*2.f, std::abs(height*s->transform->scaleVector.y)*scale*2.f);
		s->body->CreateFixture(&tShape, 1);
		b2Filter t;
		t.groupIndex = -8;
		t.categoryBits = PROP;
		t.maskBits = BOUNDARY;
		s->body->GetFixtureList()->SetFilterData(t);

		b2Vec2 v1 = tShape.GetVertex(0);
		b2Vec2 v2 = tShape.GetVertex(1);
		b2Vec2 v3 = tShape.GetVertex(2);
		b2Vec2 v4 = tShape.GetVertex(3);

		s->mesh->vertices.at(0).x = v1.x;
		s->mesh->vertices.at(0).y = v1.y;
		s->mesh->vertices.at(1).x = v2.x;
		s->mesh->vertices.at(1).y = v2.y;
		s->mesh->vertices.at(2).x = v3.x;
		s->mesh->vertices.at(2).y = v3.y;
		s->mesh->vertices.at(3).x = v4.x;
		s->mesh->vertices.at(3).y = v4.y;
	
		float mag = std::max(s->mesh->textures.at(0)->width, s->mesh->textures.at(0)->height);
		s->mesh->vertices.at(3).u = 0;
		s->mesh->vertices.at(3).v = 0;
		s->mesh->vertices.at(2).u = width/mag;
		s->mesh->vertices.at(2).v = 0;
		s->mesh->vertices.at(1).u = width/mag;
		s->mesh->vertices.at(1).v = height/mag;
		s->mesh->vertices.at(0).u = 0;
		s->mesh->vertices.at(0).v = height/mag;
		
		s->setTranslationPhysical(i * 50, 3, -3);
		s->setShader(shader, true);
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

	
	b2Filter gf;
	gf.categoryBits = BOUNDARY;
	b2Fixture * blah = ground->body->GetFixtureList();
	ground->body->GetFixtureList()->SetFilterData(gf);


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
	foregroundScreen->transform->scale(1, 10, 4.5);
	foregroundScreen->transform->translate(0, 0, 12.0f);
	
	for(Vertex & v :foregroundScreen->mesh->vertices){
		//v.x += 0.5f;
		v.u += 0.5f;
	}
	foregroundScreen->mesh->dirty = true;
	
	foregroundScreen->setShader(shader, true);

	
	Texture * font = new Texture("../assets/MoonFlowerBold.png", 1024, 1024, true, true);
	BitmapFont * fontM = new BitmapFont(font, 32, 16, 16); 
    fontM->setText("sdsdweqweqwewqesdsdsdadasd");
	fontM->transform->translate(0, 3, 5);
	fontM->setShader(shader, true);

	addChild(midgroundScreen);
	
	addChild(ground);
	addChild(foregroundScreen);
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
	
	Character1 * char1 = new Character1(world, true, CHARACTER, BOUNDARY);
	char1->setShader(shader);
	char1->addToScene(this);
	addChild(char1);
	Character2 * char2 = new Character2(world, true, CHARACTER, BOUNDARY);
	char2->setShader(shader);
	char2->addToScene(this);
	addChild(char2);
	Character3 * char3 = new Character3(world, true, CHARACTER, BOUNDARY);
	char3->setShader(shader);
	char3->addToScene(this);
	addChild(char3);
	Character4 * char4 = new Character4(world, true, CHARACTER, BOUNDARY);
	char4->setShader(shader);
	char4->addToScene(this);
	addChild(char4);
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