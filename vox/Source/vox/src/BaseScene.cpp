#pragma once

#include "BaseScene.h"
#include "Cube.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"
#include "MeshInterface.h"
#include "Texture.h"
#include "Plane.h"
#include "Keyboard.h"
#include "Camera.h"
#include "shader/ShadowShaderComponent.h"
#include "shader/PhongShaderComponent.h"
#include "shader/DiffuseShaderComponent.h"
#include "Material.h"
#include "PointLight.h"
#include "PerspectiveCamera.h"
#include "DirectionalLight.h"
#include "MeshEntity.h"
#include "Resource.h"
#include "VoxelJoint.h"
#include "shader/VoxelComponent.h"
#include "node/NodeAnimatable.h"
#include "Texture.h"
#include "VoxelMesh.h"
#include "StandardFrameBuffer.h"
#include "RenderSurface.h"
#include "Sprite.h"

BaseScene::BaseScene(Game * _game):
	Scene(_game),
	ground(new Cube(glm::vec3(0.0f, -1.5f, 0.0), 1)),
	cube(new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 1)),
	monkey(new MeshEntity()),
	shader(new BaseComponentShader()),
	material(new Material(80.0, glm::vec3(1.f, 1.f, 1.f), true)),
	frameBuffer(new StandardFrameBuffer(true)),
	sprite(new Sprite(nullptr, new Transform()))
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
	
	monkeyMesh = Resource::loadMeshFromObj("../assets/monkey.vox");
	//Load the monkeys mesh from the monkey obj
	monkey->mesh = Resource::loadMeshFromObj("../assets/monkey.vox");
	monkey->mesh->pushMaterial(material);
	monkey->setShader(shader, true);

	monkey->transform->translate(-4.0f, 0.0f, 0.0f);
	//Add the monkey to the scene
	addChild(monkey);

	//Lets create a voxel shader by using the voxel geometry component
	BaseComponentShader * voxelShader = new BaseComponentShader();
	voxelShader->components.push_back(new TextureShaderComponent());
	voxelShader->components.push_back(new ShadowShaderComponent());
	voxelShader->components.push_back(new PhongShaderComponent());
	voxelShader->geometryComponent = new VoxelComponent();
	voxelShader->compileShader();

	//Load a voxel joint from voo.json
	VoxelJoint * voxelJoint = Resource::loadVoxelModel("../assets/dong.json");

	//Lets set the shader on the joint and all of its children
	voxelJoint->setShaderOnChildren(voxelShader);

	//Lets take advantage of NodeParents doRecursivley function to set
	// material on the joint and its children
	void * args[2] = {tex, material}; 
	voxelJoint->doRecursivley([](Node * _node, void * _args[] ){
		MeshEntity * me = dynamic_cast<MeshEntity *>(_node);
		if(me != nullptr){
			if(me->mesh != nullptr){
				me->mesh->pushMaterial(static_cast<Material *>(_args[1]));	
				for(unsigned long int i = 0; i < me->mesh->vertices.size(); ++i){
					 me->mesh->vertices.at(i).red = 1.0f;
					 me->mesh->vertices.at(i).blue = 1.0f;
					 me->mesh->vertices.at(i).green = 1.0f;
				}
			}
		}
	}, args);

	//Add it to the scene
	addChild(voxelJoint);

	//Lets also make a voxel monkey using the monkey mesh that we already loaded
	MeshEntity * voxelMonkey = new MeshEntity(new VoxelMesh(monkeyMesh));
	//Add voxel shader
	voxelMonkey->setShader(voxelShader, true);
	//Add a material
	voxelMonkey->mesh->pushMaterial(material);
	voxelMonkey->transform->translate(0.0f, 0.0f, 3.0f);
	addChild(voxelMonkey);


	sprite->setShader(shader, true);
	sprite->mesh->pushTexture2D(tex);
	sprite->mesh->pushMaterial(material);
	sprite->transform->translate(-5, 0, 0);

	addChild(sprite);

	//Load the render surface shader from a file
	renderSurfaceShader = new Shader("../assets/RenderSurface", false, true);
	//Instantiate the render surface
	renderSurface = new RenderSurface(renderSurfaceShader);
}


BaseScene::~BaseScene(){
	delete cube;
	delete shader;
	delete ground;
	delete pointLight;
	delete keyLight;
}

void BaseScene::update(Step * _step){
	
	Scene::update(_step);

	//Add movement to the camera
	if(keyboard->keyDown(GLFW_KEY_W)){
		camera->transform->translate((camera->forwardVectorRotated) * static_cast<PerspectiveCamera *>(camera)->speed);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		camera->transform->translate((camera->forwardVectorRotated) * -static_cast<PerspectiveCamera *>(camera)->speed);	
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		camera->transform->translate((camera->rightVectorRotated) * -static_cast<PerspectiveCamera *>(camera)->speed);		
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		camera->transform->translate((camera->rightVectorRotated) * static_cast<PerspectiveCamera *>(camera)->speed);	
	}
	
	//Toggle fullscreen
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		Scene::toggleFullScreen();
	}
}

void BaseScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	//Render the shadows
	Scene::renderShadows(_matrixStack, _renderStack);
	frameBuffer->resize(viewPortWidth, viewPortHeight);
	//Bind frameBuffer
	frameBuffer->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderStack);

	//Render the buffer to the render surface
	renderSurface->render(frameBuffer->getTextureId());
}

void BaseScene::load(){
	frameBuffer->load();
	renderSurface->load();

	Scene::load();
}

void BaseScene::unload(){
	frameBuffer->unload();
	renderSurfaceShader->unload();

	Scene::unload();
}