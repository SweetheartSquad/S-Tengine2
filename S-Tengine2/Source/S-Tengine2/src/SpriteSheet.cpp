#pragma once

#include "SpriteSheet.h"
#include "Texture.h"
#include <SpriteSheetAnimation.h>
#include <Log.h>

SpriteSheet::SpriteSheet(Texture* _texture){
	texture = _texture;
	texture->incrementReferenceCount();
}

SpriteSheet::~SpriteSheet(){
	for(auto i : animations){
		delete i.second;
	}
	animations.clear();
	texture->decrementAndDelete();
}

void SpriteSheet::load(){
	texture->load();
	NodeLoadable::load();
}

void SpriteSheet::unload(){
	texture->unload();
	NodeLoadable::unload();
}

void SpriteSheet::addAnimation(std::string _name, SpriteSheetAnimationDefinition * _animation){
	auto res = animations.insert(std::pair<std::string, SpriteSheetAnimationDefinition * >(_name, _animation));
	if(!res.second){
		Log::error("Animation with name \""+_name+"\" already exists; not added.");
	}
}

void SpriteSheet::addAnimation(std::string _name, unsigned long int _start, unsigned long int _end, float _width, float _height, float _secondsPerFrame){
	SpriteSheetAnimationDefinition * ssa = new SpriteSheetAnimationDefinition(_secondsPerFrame);
	ssa->pushFramesInRange(_start, _end, _width, _height, texture->width, texture->height);
	addAnimation(_name, ssa);
}