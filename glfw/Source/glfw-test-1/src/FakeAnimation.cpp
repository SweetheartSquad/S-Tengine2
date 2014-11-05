#pragma once

#include "FakeAnimation.h"
#include "Resource.h"
#include "Entity.h"

void FakeAnimation::pushFrame(Entity * _frame){
	_frame->transform = this->transform;
	_frame->shader = this->shader;
	_frame->parent = nullptr;//this;
	_frame->reset();
	children.push_back(_frame);
}

FakeAnimation::FakeAnimation(Transform * t, Shader * s, Entity * parent) :
	Entity(nullptr, t, s, parent),
	frame(0),
	delay(3),
	delayCount(0)
{
}

FakeAnimation::~FakeAnimation(){
	for(unsigned long int i = 0; i < children.size(); ++i){
		delete children.at(i);
	}
	children.clear();

	delete transform;
	delete shader;
}

void FakeAnimation::draw(MatrixStack * _matrixStack, std::vector<Light*> _lights){
	//push transform
	_matrixStack->pushMatrix();
	_matrixStack->applyMatrix(transform->getModelMatrix());

	//for(Entity * child : children){
		dynamic_cast<Entity *>(children.at(frame))->draw(_matrixStack, _lights);
	//}

	//pop transform
	_matrixStack->popMatrix();
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