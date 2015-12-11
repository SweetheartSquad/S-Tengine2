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
#include <Log.h>

struct b2Vec2;

Sprite::Sprite(Shader * _shader) :
	MeshEntity(MeshFactory::getPlaneMesh(), _shader),
	currentAnimation(nullptr),
	playAnimation(true)
{
}

Sprite::Sprite(Texture * _texture, Shader * _shader) :
	MeshEntity(MeshFactory::getPlaneMesh(), _shader),
	currentAnimation(nullptr),
	playAnimation(true)
{
	setPrimaryTexture(_texture);
}

Sprite::Sprite(TextureSampler * _textureSampler, Shader * _shader) :
	MeshEntity(MeshFactory::getPlaneMesh(), _shader),
	currentAnimation(nullptr),
	playAnimation(true)
{
	setPrimaryTexture(_textureSampler);
}


Sprite::~Sprite(){
	for(auto i : animations){
		delete i.second;
	}
	animations.clear();
}

void Sprite::update(Step* _step){
	MeshEntity::update(_step);
	if(currentAnimation != nullptr && playAnimation){
		mesh->dirty = true;
		currentAnimation->update(_step);
		setUvs(currentAnimation->frames.at(currentAnimation->currentFrame));
		if(mesh->textures.size() > 0){
			mesh->textures.at(0) = currentAnimation->texture;
		}else {
			mesh->pushTexture2D(currentAnimation->texture);
		}
	}
}

void Sprite::addAnimation(std::string _name, SpriteSheetAnimation* _animation, bool _makeCurrent){
	auto res = animations.insert(std::pair<std::string, SpriteSheetAnimation * >(_name, _animation));
	if(!res.second){
		Log::error("Animation with name "+_name+" already exists; not added.");
	}
	if(_makeCurrent){
		currentAnimation = _animation;
	}
}

void Sprite::setPrimaryTexture(Texture * _texture) {
	if(mesh->textures.size() == 0) {
		mesh->pushTexture2D(_texture);
	}else {
		mesh->textures[0] = _texture;
		mesh->texturesDirty = true;
		++_texture->referenceCount;
	}
	
	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);

	float width = _texture->width;
	float height = _texture->height;
	
	float negHeight = -height/mag/2;
	float negWidth  = -width/mag/2;
	float posHeight =  height/mag/2;;
	float posWidth  =  width/mag/2;;

	mesh->vertices.at(0).x = negWidth;
	mesh->vertices.at(0).y = posHeight;
	mesh->vertices.at(1).x = posWidth;
	mesh->vertices.at(1).y = posHeight;
	mesh->vertices.at(2).x = posWidth;
	mesh->vertices.at(2).y = negHeight;
	mesh->vertices.at(3).x = negWidth;
	mesh->vertices.at(3).y = negHeight;

	mesh->dirty = true;
}

void Sprite::setPrimaryTexture(TextureSampler * _textureSampler) {

	if(mesh->textures.size() == 0) {
		mesh->pushTexture2D(_textureSampler->texture);
	}else {
		mesh->textures[0] = _textureSampler->texture;
		mesh->texturesDirty = true;
		++_textureSampler->texture->referenceCount;
	}

	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);

	float u = _textureSampler->u;
	float v = _textureSampler->v;
	float width = _textureSampler->width;
	float height = _textureSampler->height;

	mesh->vertices.at(0).u = u/mag;
	mesh->vertices.at(0).v = v/mag;
	mesh->vertices.at(1).u = (u + width)/mag;
	mesh->vertices.at(1).v = v/mag;
	mesh->vertices.at(2).v = (v + height)/mag;
	mesh->vertices.at(2).u = (u + width)/mag;
	mesh->vertices.at(3).u = u/mag;
	mesh->vertices.at(3).v = (v + height)/mag;
	
	float negHeight = -height/mag/2;
	float negWidth  = -width/mag/2;
	float posHeight =  height/mag/2;;
	float posWidth  =  width/mag/2;;

	mesh->vertices.at(0).x = negWidth;
	mesh->vertices.at(0).y = posHeight;
	mesh->vertices.at(1).x = posWidth;
	mesh->vertices.at(1).y = posHeight;
	mesh->vertices.at(2).x = posWidth;
	mesh->vertices.at(2).y = negHeight;
	mesh->vertices.at(3).x = negWidth;
	mesh->vertices.at(3).y = negHeight;

	mesh->dirty = true;
}

void Sprite::setCurrentAnimation(std::string _name){
	auto anim = animations.find(_name);
	if(anim != animations.end()){
		currentAnimation = anim->second;
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

void Sprite::setUvs(sweet::Rectangle _rect){
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