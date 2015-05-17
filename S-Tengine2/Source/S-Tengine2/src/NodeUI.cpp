#pragma once

#include <NodeUI.h>
#include <Scene.h>
#include <Camera.h>
#include <Mouse.h>

NodeUI::NodeUI(BulletWorld * _world, Scene * _scene) :
	NodeBulletBody(_world),
	scene(_scene),
	isHovered(false),
	isDown(false),
	isActive(false),
	mouse(&Mouse::getInstance())
{

}


void NodeUI::down(){
	isHovered = true;
	isDown = true;
	
    // do event stuff
}
void NodeUI::up(){
	isDown = false;
	
	// do event stuff
}
void NodeUI::in(){
	isHovered = true;
}
void NodeUI::out(){
	isHovered = false;
}


void NodeUI::update(Step * _step){
	
	float raylength = 1000;

	Camera * cam = scene->activeCamera;
		
	glm::vec3 campos = cam->getWorldPos();
	glm::vec3 camdir = cam->forwardVectorRotated;
	btVector3 raystart(campos.x, campos.y, campos.z);
	btVector3 raydir(camdir.x, camdir.y, camdir.z);
	btVector3 rayend = raystart + raydir*raylength;
	
	/*btCollisionWorld::ClosestRayResultCallback RayCallback(raystart, rayend);
	world->world->rayTest(raystart, rayend, RayCallback);
	if(RayCallback.hasHit()){

	}*/
	btTransform rayfrom;
	rayfrom.setIdentity(); rayfrom.setOrigin(raystart);
	btTransform rayto;
	rayto.setIdentity(); rayto.setOrigin(rayend);
	btCollisionWorld::AllHitsRayResultCallback raycb(raystart, rayend);
	world->world->rayTestSingle(rayfrom, rayto, body, shape, body->getWorldTransform(), raycb);
	
	
	if(raycb.hasHit()){
		if(!isHovered){
			in();
		}if(mouse->leftJustPressed()){
			down();
		}else if(mouse->leftJustReleased()){
			up();
		}
	}else{
		if(isHovered){
			out();
		}
		if(isDown && mouse->leftJustReleased()){
			isDown = false;
		}
	}

	NodeBulletBody::update(_step);
	Entity::update(_step);
}

void NodeUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	if(isHovered){
		if(isDown){
			renderDown(_matrixStack, _renderOptions);
		}else{
			renderOver(_matrixStack, _renderOptions);
		}
	}else if(isActive){
		renderActive(_matrixStack, _renderOptions);
	}else{
		renderDefault(_matrixStack, _renderOptions);
	}
}

void NodeUI::renderDown(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "down" << std::endl;
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::renderOver(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "over" << std::endl;
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::renderActive(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
//	std::cout << "active" << std::endl;
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::renderDefault(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "default" << std::endl;
	Entity::render(_matrixStack, _renderOptions);
}