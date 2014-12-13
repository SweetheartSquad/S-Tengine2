#pragma once

#include "BaseScene.h"
#include "Cube.h"
#include "BaseComponentShader.h"
#include "TextureShaderComponent.h"
#include "MeshInterface.h"
#include "Texture.h"
#include "Plane.h"
#include "Keyboard.h"
#include "Camera.h"
#include "ShadowShaderComponent.h"
#include "PhongShaderComponent.h"
#include "DiffuseShaderComponent.h"
#include "Material.h"
#include "PointLight.h"
#include "PerspectiveCamera.h"

BaseScene::BaseScene(Game* _game):
	Scene(_game),
	ground(new Cube(glm::vec3(0.0f, -2.0f, 0.0), 1)),
	cube(new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 1)),
	shader(new BaseComponentShader()),
	material(new Material(80.0, glm::vec3(1.f, 1.f, 1.f), true)),
	light(new PointLight(glm::vec3(1.f, -1.5, 1.f),
		glm::vec3(0.1f, 0.2f, 0.8f),
		0.2f, 0.005f))
{
	//Add shader components
	shader->components.push_back(new TextureShaderComponent());
	shader->components.push_back(new PhongShaderComponent());
	shader->components.push_back(new ShadowShaderComponent());
	
	//Compile the shader
	shader->compileShader();

	//Set the shader on cube
	cube->setShader(shader, true);
	//Since we are using a phong component the cube needs a material
	cube->mesh->pushMaterial(material);
	//Add it as a child of the scene
	addChild(cube);

	Texture * tex = new Texture("../assets/uv-test.jpg", 1000, 1000, true, true);
	cube->mesh->pushTexture2D(tex);

	//Use the same shader for the ground
	ground->setShader(shader, true);
	//set the material on the ground
	ground->mesh->pushMaterial(material);
	//Make the ground brown
	ground->setColour(0.65f, 0.16f, 0.16f, 1.0f);
	//Rotate the ground 90 degrees along the X-axis so it is flat
	ground->transform->scale(5.0f, 1.0f, 5.0f);
	//Add the texture to ground
	ground->mesh->pushTexture2D(tex);
	//Add it to the scene
	addChild(ground);
	
	//Setup light
	PointLight * light =  new PointLight(glm::vec3(1.f, -1.5, 1.f),
		glm::vec3(0.1f, 0.2f, 0.8f),
		0.2f, 0.005f);

	light->transform->translate(-2.0f, 3.0f, 0.0f);
	//Make the light bright white
	light->data.intensities = glm::vec3(1.0f, 1.0f, 1.0f);
	light->data.attenuation = 0.2f;
	light->data.ambientCoefficient = 0.005f;

	//Add it to the scene's list of lights
	lights.push_back(light);
}

BaseScene::~BaseScene(){
	delete cube;
	delete shader;
	delete ground;
	delete light;
}

void BaseScene::update(){
	
	Scene::update();

	//Add movement to the camera
	if(keyboard->keyDown(GLFW_KEY_W)){
		camera->transform->translate((camera->forwardVectorRotated) * camera->speed);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		camera->transform->translate((camera->forwardVectorRotated) * -camera->speed);	
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		camera->transform->translate((camera->rightVectorRotated) * -camera->speed);		
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		camera->transform->translate((camera->rightVectorRotated) * camera->speed);	
	}
	
	//Toggle fullscreen
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		Scene::toggleFullScreen();
	}
}

void BaseScene::render(){
	Scene::renderShadows();
	Scene::render();
}