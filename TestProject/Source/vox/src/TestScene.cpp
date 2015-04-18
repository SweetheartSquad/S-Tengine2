#pragma once

#include <TestScene.h>

#include <TestGame.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <Material.h>

#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentPhong.h>
#include <shader\ShaderComponentBlinn.h>
#include <shader\ShaderComponentShadow.h>

#include <Box2DWorld.h>
#include <Box2DMeshEntity.h>
#include <Box2DDebugDraw.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <MatrixStack.h>

TestScene::TestScene(Game * _game) :
	Scene(_game),
	shader(new BaseComponentShader()),
	world(new Box2DWorld(b2Vec2(0,0))),
	drawer(nullptr),
	player(nullptr)
{
	shader->components.push_back(new ShaderComponentTexture(shader));
	//shader->components.push_back(new ShaderComponentPhong(shader));
	shader->components.push_back(new ShaderComponentDiffuse(shader));
	//shader->components.push_back(new ShaderComponentShadow(shader));
	shader->compileShader();

	
	//Set up cameras
	mouseCam = new MousePerspectiveCamera();
	cameras.push_back(mouseCam);
	mouseCam->farClip = 1000.f;
	mouseCam->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCam->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCam->yaw = 90.0f;
	mouseCam->pitch = -10.0f;
	mouseCam->speed = 1;

	gameCam = new FollowCamera(15, glm::vec3(0, 0, 0), 0, 0);
	cameras.push_back(gameCam);
	gameCam->farClip = 1000.f;
	gameCam->transform->rotate(90, 0, 1, 0, kWORLD);
	gameCam->transform->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	activeCamera = gameCam;
	
	clearColor[0] = 0.5f;


	float sceneHeight = 150;
	float sceneWidth = 150;
	float _size = 3;
	std::vector<Box2DMeshEntity *> boundaries;
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, MeshFactory::getPlaneMesh(), b2_staticBody));

	boundaries.at(0)->transform->scale(_size, sceneHeight*0.5f + _size*2.f, _size);
	boundaries.at(1)->transform->scale(_size, sceneHeight*0.5f + _size*2.f, _size);
	boundaries.at(2)->transform->scale(sceneWidth*0.5f + _size*2.f, _size, _size);
	boundaries.at(3)->transform->scale(sceneWidth*0.5f + _size*2.f, _size, _size);

	boundaries.at(0)->setTranslationPhysical(sceneWidth+_size, sceneHeight*0.5f, 0);
	boundaries.at(1)->setTranslationPhysical(-_size, sceneHeight*0.5f, 0);
	boundaries.at(2)->setTranslationPhysical(sceneWidth*0.5f, sceneHeight+_size, 0);
	boundaries.at(3)->setTranslationPhysical(sceneWidth*0.5f, -_size, 0);
	
	b2Filter sf;
	//sf.categoryBits = PuppetGame::kBOUNDARY;
	//sf.maskBits = -1;
	for(auto b : boundaries){
		addChild(b);
		b->setShader(shader, true);
		world->addToWorld(b);
		b->body->GetFixtureList()->SetFilterData(sf);
		//b->mesh->pushTexture2D(PuppetResourceManager::stageFront);
	}
	//sf.categoryBits = PuppetGame::kBOUNDARY | PuppetGame::kGROUND;
	boundaries.at(3)->body->GetFixtureList()->SetFilterData(sf);
	boundaries.at(3)->body->GetFixtureList()->SetFriction(1);
	boundaries.at(3)->body->GetFixtureList()->SetRestitution(0);



	//lights.push_back(new DirectionalLight(glm::vec3(1,0,0), glm::vec3(1,1,1), 1));
	
	

	//intialize key light
	DirectionalLight * keyLight = new DirectionalLight(glm::vec3(0.5f, 0.8f, 0.6f), glm::vec3(1.f, 1.f, 1.f), 1.005f);
	//Set it as the key light so it casts shadows
	keyLight->isKeyLight = true;
	//Add it to the scene
	lights.push_back(keyLight);

	Material * phong = new Material(45.0, glm::vec3(1.f, 1.f, 0.f), true);
	{
	Box2DMeshEntity * m = new Box2DMeshEntity(world, MeshFactory::getCubeMesh(), b2_dynamicBody, false);
	m->transform->translate(15,0,0);
	m->mesh->pushMaterial(phong);
	world->addToWorld(m);
	m->createFixture();
	player = m;
	m->setShader(shader, true);
	addChild(m);
	gameCam->addTarget(player, 1);
	PointLight * pointLight = new PointLight(glm::vec3(0.0f, 0.0f, 5.f), glm::vec3(0.0f, 0.0f, 1.0f), 0.005f, 0.2f);
	//Add it to the scene's list of lights
	lights.push_back(pointLight);
	m->addChild(pointLight);
	}

	{
	Box2DMeshEntity * m = new Box2DMeshEntity(world, Resource::loadMeshFromObj("../assets/torus.obj"), b2_dynamicBody, false);
	m->transform->scale(1, 1, 1);
	m->mesh->pushMaterial(phong);
	world->addToWorld(m);
	m->createFixture();
	m->setShader(shader, true);
	addChild(m);
	}
}

TestScene::~TestScene(){
	delete world;
}

void TestScene::update(Step * _step){
	
	shader->components.at(1)->makeDirty();

	clearColor[0] = sin(_step->time)*0.5f + 0.5f;
	clearColor[2] = cos(_step->time)*0.5f + 0.5f;

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}

	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	
	// player controls
	if(player != nullptr){
		if (keyboard->keyDown(GLFW_KEY_W)){
			player->applyLinearImpulseUp(5);
		}
		if (keyboard->keyDown(GLFW_KEY_S)){
			player->applyLinearImpulseDown(5);
		}
		if (keyboard->keyDown(GLFW_KEY_A)){
			player->applyLinearImpulseLeft(5);
		}
		if (keyboard->keyDown(GLFW_KEY_D)){
			player->applyLinearImpulseRight(5);
		}
	}


	// debug controls
	if(keyboard->keyJustDown(GLFW_KEY_1)){
		if(activeCamera == gameCam){
			activeCamera = mouseCam;
		}else{
			activeCamera = gameCam;
		}
	}
	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(drawer != nullptr){
			world->b2world->SetDebugDraw(nullptr);
			removeChild(drawer);
			delete drawer;
			drawer = nullptr;
		}else{
			drawer = new Box2DDebugDraw(this, world);
			world->b2world->SetDebugDraw(drawer);
			drawer->drawing = true;
			//drawer->AppendFlags(b2Draw::e_aabbBit);
			drawer->AppendFlags(b2Draw::e_shapeBit);
			drawer->AppendFlags(b2Draw::e_centerOfMassBit);
			drawer->AppendFlags(b2Draw::e_jointBit);
			//drawer->AppendFlags(b2Draw::e_pairBit);
			addChild(drawer);
		}
	}

	Scene::update(_step);

	world->update(_step);
}

void TestScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Scene::render(_matrixStack, _renderOptions);
}