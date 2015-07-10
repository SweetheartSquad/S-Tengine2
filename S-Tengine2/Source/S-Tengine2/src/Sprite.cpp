#pragma once

#include "Sprite.h"
#include "Rectangle.h"
#include "Point.h"
#include "SpriteSheetAnimation.h"
#include "Rectangle.h"
#include "Box2DSuperSprite.h"
#include <Texture.h>
#include <TextureSampler.h>
#include <MeshFactory.h>
#include <MeshInterface.h>

struct b2Vec2;

Sprite::Sprite(Shader * _shader) :
	MeshEntity(MeshFactory::getPlaneMesh()),
	NodeShadable(_shader),
	currentAnimation(nullptr),
	animatedTexture(nullptr),
	playAnimation(true)
{
}

Sprite::~Sprite(){
}

void Sprite::update(Step* _step){
	MeshEntity::update(_step);
	if(currentAnimation != nullptr && playAnimation){
		currentAnimation->update(_step);
		mesh->dirty = true;
		setUvs(currentAnimation->frames.at(currentAnimation->currentFrame));
		animatedTexture = currentAnimation->texture;
	}
}

void Sprite::addAnimation(std::string _name, SpriteSheetAnimation* _animation, bool _makeCurrent){
	animations.insert(std::make_pair(_name, _animation));
	if(_makeCurrent){
		currentAnimation = _animation;
	}
}

void Sprite::setCurrentAnimation(std::string _name){
	SpriteSheetAnimation * anim = animations.at(_name);
	if(anim != nullptr){
		currentAnimation = anim;
	}
}

Vertex * Sprite::getTopLeft(){
	return &mesh->vertices.at(0);
}

Vertex * Sprite::getTopRight(){
	return &mesh->vertices.at(1);
}

Vertex * Sprite::getBottomLeft(){
	return &mesh->vertices.at(3);
}

Vertex * Sprite::getBottomRight(){
	return &mesh->vertices.at(2);
}

void Sprite::setUvs(float _topLeftU, float _topLeftV, float _topRightU, float _topRightV,
					float _bottomLeftU, float _bottomLeftV, float _bottomRightU, float _bottomRightV){
	getBottomLeft()->u    = _bottomLeftU;
	getBottomLeft()->v    = _bottomLeftV;
	getTopLeft()->u       = _topLeftU;
	getTopLeft()->v       = _topLeftV;
	getTopRight()->u      = _topRightU;
	getTopRight()->v      = _topRightV;
	getBottomRight()->u   = _bottomRightU;
	getBottomRight()->v   = _bottomRightV;
}

void Sprite::setUvs(vox::Rectangle _rect){
	getTopLeft()->u  = _rect.getTopLeft().x;
	getTopLeft()->v  = _rect.getTopLeft().y;
	getTopRight()->u  = _rect.getTopRight().x;
	getTopRight()->v  = _rect.getTopRight().y;
	getBottomRight()->u  = _rect.getBottomRight().x;
	getBottomRight()->v  = _rect.getBottomRight().y;
	getBottomLeft()->u  = _rect.getBottomLeft().x;
	getBottomLeft()->v  = _rect.getBottomLeft().y;
}

void Sprite::load(){
	if(!loaded){
		for(auto s : animations) {
			s.second->load();
		}
		
	}
	MeshEntity::load();
}

void Sprite::unload(){
	if(loaded){
		for(auto s : animations) {
			s.second->unload();
		}	
	}
	MeshEntity::unload();
}