#pragma once

#include "SpriteSheetAnimation.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Tween.h"
#include "Rectangle.h"
#include "Texture.h"

SpriteSheetAnimation::SpriteSheetAnimation(Texture * _texture, float _secondsPerFrame):
	NodeUpdatable(),
	frameIndicies(Animation<int>(&currentFrame)),
	texture(_texture),
	currentFrame(0),
	secondsPerFrame(_secondsPerFrame)
{
	frameIndicies.hasStart = true;
}

SpriteSheetAnimation::~SpriteSheetAnimation(){
}

void SpriteSheetAnimation::update(Step* _step){
	frameIndicies.update(_step);
	currentFrame = frameIndicies.currentTween;
}

void SpriteSheetAnimation::pushFrame(int _column, int _row, float _width, float _height){
	float u, v, rW, rH;
	rW = _width / texture->width;
	rH = _height / texture->height;
	u = rW * _column;
	v = rH * _row;
	frames.push_back(Rectangle(u, v, rW, rH));
	frameIndicies.tweens.push_back(new Tween<int>(secondsPerFrame, 1, Easing::kNONE));
}

void SpriteSheetAnimation::pushMultipleFrames(std::vector<int> _frames, float _width, float _height){
	pushMultipleFrames(_frames, _width, _height, texture->width);
}

void SpriteSheetAnimation::pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth){
	int curCol = 0;
	int curRow = 0;
	int colOffset = 0;
	int colInRow = -1;
	for(unsigned long int i = 0; i < _frames.size(); i++){
		curCol = _frames.at(i);
		if((curCol - colOffset + 1) * _width > _textureWidth){
			curRow++;
			if(colInRow == -1){
				colInRow = curCol;	
			}
			colOffset += colInRow;
		}
		pushFrame(curCol - colOffset, curRow, _width, _height);
	}
}

void SpriteSheetAnimation::pushFramesInRange(int _min, int _max, float _width, float _height, float _textureWidth){
	int curCol = _min - 1;
	int curRow = 0;
	int colOffset = 0;
	int colInRow = -1;
	for(unsigned long int i = 0; i <= _max - _min; i++){
		curCol++;
		if((curCol - colOffset + 1) * _width > _textureWidth){
			curRow++;
			if(colInRow == -1){
				colInRow = curCol;	
			}
			colOffset += colInRow;
		}
		pushFrame(curCol - colOffset, curRow, _width, _height);
	}
}

void SpriteSheetAnimation::pushFramesInRange(int _min, int _max, float _width, float _height){
	pushFramesInRange(_min, _max, _width, _height, texture->width);
}