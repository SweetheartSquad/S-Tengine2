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
	frames.push_back(Rectangle(u, v, -rW, rH));
	frameIndicies.tweens.push_back(new Tween<int>(secondsPerFrame, 1, Easing::kNONE));
}

void SpriteSheetAnimation::pushMultipleFrames(int _frames[], float _width, float _height){
	int curCol = 0;
	int curRow = 0;
	for(unsigned long int i = 0; i < sizeof(frames)/sizeof(int); i++){
		curCol = _frames[i];
		if(curCol / (curRow + 1) * _width > texture->width){
			curRow++;
			curCol /= (curRow + 1);
		}
		pushFrame(curCol, curRow, _width, _height);
	}
}

void SpriteSheetAnimation::pushFramesInRange(int _min, int _max, float _width, float _height){
	int curCol = _min;
	int curRow = 0;
	for(unsigned long int i = 0; i <= _max - _min; i++){
		if(curCol / (curRow + 1) * _width > texture->width){
			curRow++;
			curCol /= (curRow + 1);
		}
		pushFrame(curCol, curRow, _width, _height);
		curCol++;
	}
}