#pragma once

#include <TestScene.h>


#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <Material.h>

#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentPhong.h>
#include <shader\ShaderComponentShadow.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <Keyboard.h>
#include <GLFW\glfw3.h>

TestScene::TestScene(Game * _game) :
	Scene(_game),
	shader(new BaseComponentShader())
{
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->components.push_back(new ShaderComponentPhong(shader));
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
	
	//lights.push_back(new DirectionalLight(glm::vec3(1,0,0), glm::vec3(1,1,1), 1));
	
	
	//Setup point light - lets make it red
	Light * pointLight = new PointLight(glm::vec3(-5.0f, 1.0f, 1.f), glm::vec3(0.8f, 0.0f, 0.0f), 0.005f, 0.2f);
	//Add it to the scene's list of lights
	lights.push_back(pointLight);

	//intialize key light
	DirectionalLight * keyLight = new DirectionalLight(glm::vec3(0.5f, 0.8f, 0.6f), glm::vec3(0.1f, 0.1f, 0.5f), 0.005f);
	//Set it as the key light so it casts shadows
	keyLight->isKeyLight = true;
	//Add it to the scene
	lights.push_back(keyLight);

	Material * phong = new Material(80.0, glm::vec3(1.f, 1.f, 1.f), true);
	{
	MeshEntity * m = new MeshEntity(MeshFactory::getCubeMesh());
	m->setShader(shader, true);
	m->transform->translate(15,0,0);
	m->mesh->pushMaterial(phong);
	addChild(m);
	}
	{
	MeshEntity * m = new MeshEntity(Resource::loadMeshFromObj("../assets/thing.vox"));
	m->setShader(shader, true);
	m->transform->scale(10, 10, 10);
	m->mesh->pushMaterial(phong);
	addChild(m);
	}
}

TestScene::~TestScene(){

}

void TestScene::update(Step * _step){
	
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

	if(keyboard->keyJustDown(GLFW_KEY_1)){
		if(activeCamera == gameCam){
			activeCamera = mouseCam;
		}else{
			activeCamera = gameCam;
		}
	}



	Scene::update(_step);
}