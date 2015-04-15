#pragma once

#include <TestScene.h>


#include <MeshEntity.h>
#include <MeshFactory.h>

#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <Keyboard.h>
#include <GLFW\glfw3.h>

TestScene::TestScene(Game * _game) :
	Scene(_game),
	shader(new BaseComponentShader())
{
	shader->components.push_back(new ShaderComponentTexture(shader));
	//shader->components.push_back(new ShaderComponentHsv(shader, 0.f, 1.25f, 1.25f));
	//shader->components.push_back(new ShaderComponentTint(shader, 0.f, 0.f, 0.f));
	//shader->components.push_back(new ShaderComponentAlpha(shader, 1.f));
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

	MeshEntity * m = new MeshEntity(MeshFactory::getCubeMesh());
	m->setShader(shader, true);
	addChild(m);
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