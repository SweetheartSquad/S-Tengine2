#pragma once 

#include "Sprite.h"
#include "Rectangle.h"
#include "Point.h"
#include "SpriteSheetAnimation.h"
#include "SpriteMesh.h"

Sprite::Sprite(Shader * _shader, Transform * _transform):
	MeshEntity(new SpriteMesh(GL_STATIC_DRAW)),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	currentAnimation(nullptr),
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
		dynamic_cast<SpriteMesh *>(mesh)->animatedTexture = currentAnimation->texture;
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

void Sprite::setUvs(Rectangle _rect){	
	mesh->vertices.at(0).u  = _rect.getTopLeft().x;
	mesh->vertices.at(0).v  = _rect.getTopLeft().y;
	mesh->vertices.at(1).u  = _rect.getTopRight().x;
	mesh->vertices.at(1).v  = _rect.getTopRight().y;
	mesh->vertices.at(2).u  = _rect.getBottomRight().x;
	mesh->vertices.at(2).v  = _rect.getBottomRight().y;
	mesh->vertices.at(3).u  = _rect.getBottomLeft().x;
	mesh->vertices.at(3).v  = _rect.getBottomLeft().y;
}

