#pragma once

#include "Sprite.h"
#include "Rectangle.h"
#include "Point.h"
#include "SpriteSheet.h"
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
	// TODO: memory management of spritesheet?
	
	// delete the current animation (this animation should be a copy of one from a spritesheet and not the original, which means we have to delete it here or it will be leaked)
	if(currentAnimation != nullptr){
		delete currentAnimation;
	}
}

void Sprite::update(Step* _step){
	MeshEntity::update(_step);
	if(currentAnimation != nullptr && playAnimation){
		mesh->dirty = true;
		currentAnimation->update(_step);
		setUvs(currentAnimation->definition->frames.at(currentAnimation->currentFrame));
		while(mesh->textures.size() > 0){
			mesh->textures.back()->decrementAndDelete();
			mesh->textures.pop_back();
		}
		mesh->pushTexture2D(spriteSheet->texture);
	}
}

void Sprite::setPrimaryTexture(Texture * _texture) {
	if(mesh->textures.size() == 0) {
		mesh->pushTexture2D(_texture);
	}else {
		mesh->textures[0] = _texture;
		_texture->incrementReferenceCount();
	}
	
	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);

	float width = _texture->width;
	float height = _texture->height;
	
	float posHeight =  height/mag/2;
	float posWidth  =  width/mag/2;
	float negHeight = -posHeight;
	float negWidth  = -posWidth;

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
		_textureSampler->texture->incrementReferenceCount();
	}

	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);

	float u = _textureSampler->u;
	float v = _textureSampler->v;
	float width = _textureSampler->width;
	float height = _textureSampler->height;


	setUvs(sweet::Rectangle(u/mag, v/mag, width/mag, height/mag));
	
	float posHeight =  height/mag/2;
	float posWidth  =  width/mag/2;
	float negHeight = -posHeight;
	float negWidth  = -posWidth;

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

void Sprite::setSpriteSheet(SpriteSheet * _spriteSheet, std::string _currentAnimation){
	spriteSheet = _spriteSheet;
	setCurrentAnimation(_currentAnimation);
}


void Sprite::setCurrentAnimation(std::string _name){
	// find the animation with the given _name
	auto anim = spriteSheet->animations.find(_name);
	if(anim == spriteSheet->animations.end()){
		Log::error("Animation with name \""+_name+"\" does not exist.");
	}

	
		
	if(currentAnimation != nullptr){
		// if it's the same animation we're already using, ignore it
		if(anim->second == currentAnimation->definition){
			return;
		}

		// delete the current animation
		delete currentAnimation;
	}

	// copy the new animation and set it
	currentAnimation = new SpriteSheetAnimationInstance(anim->second);
}

Vertex * Sprite::getTopLeft(){
	return &mesh->vertices.at(0);
}

Vertex * Sprite::getTopRight(){
	return &mesh->vertices.at(1);
}

Vertex * Sprite::getBottomRight(){
	return &mesh->vertices.at(2);
}

Vertex * Sprite::getBottomLeft(){
	return &mesh->vertices.at(3);
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
	mesh->dirty = true;
}

void Sprite::setUvs(sweet::Rectangle _rect){
	setUvs(
		_rect.getTopLeft().x, _rect.getTopLeft().y,
		_rect.getTopRight().x, _rect.getTopRight().y,
		_rect.getBottomLeft().x, _rect.getBottomLeft().y,
		_rect.getBottomRight().x, _rect.getBottomRight().y
	);
}