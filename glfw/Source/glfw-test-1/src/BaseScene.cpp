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
#include "DirectionalLight.h"
#include "MeshEntity.h"
#include "Resource.h"
#include "VoxelJoint.h"
#include <VoxelComponent.h>
#include "NodeAnimatable.h"

BaseScene::BaseScene(Game* _game):
	Scene(_game),
	ground(new Cube(glm::vec3(0.0f, -1.5f, 0.0), 1)),
	cube(new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 1)),
	shader(new BaseComponentShader()),
	material(new Material(80.0, glm::vec3(1.f, 1.f, 1.f), true)),
	monkey(new MeshEntity())
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
	ground->transform->scale(15.0f, 0.5f, 15.0f);
	//Add the texture to ground
	ground->mesh->pushTexture2D(tex);
	//Add it to the scene
	addChild(ground);
	
	//Setup point light - lets make it red
	pointLight = new PointLight(glm::vec3(-5.0f, 1.0f, 1.f), glm::vec3(0.8f, 0.0f, 0.0f), 0.005f, 0.2f);
	//Add it to the scene's list of lights
	lights.push_back(pointLight);

	//intialize key light
	keyLight = new DirectionalLight(glm::vec3(0.5f, 0.8f, 0.6f), glm::vec3(0.1f, 0.1f, 0.5f), 0.005f);
	//Set it as the key light so it casts shadows
	keyLight->isKeyLight = true;
	//Add it to the scene
	lights.push_back(keyLight);
	
	//Load the monkeys mesh from the monkey obj
	monkey->mesh = Resource::loadMeshFromObj("../assets/monkey.vox");
	monkey->mesh->pushMaterial(material);
	monkey->setShader(shader, true);

	monkey->transform->translate(-4.0f, 0.0f, 0.0f);

	addChild(monkey);

	BaseComponentShader * vs = new BaseComponentShader();
	vs->components.push_back(new TextureShaderComponent());
	vs->components.push_back(new ShadowShaderComponent());
	vs->geometryComponent = new VoxelComponent();
	vs->compileShader();

	VoxelJoint * j = Resource::loadVoxelModel("../assets/voo.json");

	NodeAnimatable * t = dynamic_cast<NodeAnimatable *>(j->children.at(0));

	t;

	j->setShaderOnChildren(vs);
	//j->mesh->pushMaterial(material);
	
	addChild(j);
}

BaseScene::~BaseScene(){
	delete cube;
	delete shader;
	delete ground;
	delete pointLight;
	delete keyLight;
}

void BaseScene::update(){
	
	NodeAnimatable * t = dynamic_cast<NodeAnimatable *>(children.back()->children.at(0));

	t;

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