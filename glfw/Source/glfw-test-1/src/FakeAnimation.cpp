#pragma once

#include "FakeAnimation.h"
#include "Resource.h"
#include "Entity.h"

FakeAnimation::FakeAnimation(Transform * t, Shader * s, Entity * parent) :
	Entity(nullptr, t, s, parent),
	frame(0),
	delay(3),
	delayCount(0)
{
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/01.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/02.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/03.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/04.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/05.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/06.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/07.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/08.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/09.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/10.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/11.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/12.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/13.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/14.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/15.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/16.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/17.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/18.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/19.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/20.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/21.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/22.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/23.obj"), transform, shader, nullptr));
	children.push_back(new Entity(Resource::loadMeshFromObj("../assets/CatThing/24.obj"), transform, shader, nullptr));

	for(Entity * e : children){
		e->mesh->polygonalDrawMode = GL_POINTS;
	}
}

FakeAnimation::~FakeAnimation(){
	for(unsigned long int i = 0; i < children.size(); ++i){
		delete children.at(i);
	}
	children.clear();

	delete transform;
	delete shader;
}

void FakeAnimation::draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	//push transform
	vox::pushMatrix();
	vox::applyMatrix(transform->getModelMatrix());

	//for(Entity * child : children){
		children.at(frame)->draw(_projectionMatrix, _viewMatrix, _lights);
	//}

	//pop transform
	vox::popMatrix();
}

void FakeAnimation::update(){
	delayCount += 1;
	if(delayCount >= delay){
		delayCount = 0;
		frame += 1;
	}
	if(frame >= children.size()){
		frame = 0;
	}
}