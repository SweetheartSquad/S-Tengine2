#pragma once

#include "SpriteSheet.h"
#include "Texture.h"
#include "Rectangle.h"

SpriteSheet::SpriteSheet(Texture* _texture){
	texture = _texture;
}

SpriteSheet::~SpriteSheet(){
}

void SpriteSheet::pushFrame(int _idx, float _width, float _height){
	float u, v, rW, rH;
	rW = _width / texture->width;
	rH = _height / texture->height;
	u = rW;
	v = rH;
	u *= _idx;
	v *= _idx;
	frames.push_back(Rectangle(u, v, rW, rH));
}