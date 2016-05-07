#pragma once

#include "SpriteSheetAnimation.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Tween.h"
#include "Rectangle.h"
#include "Texture.h"

SpriteSheetAnimationDefinition::SpriteSheetAnimationDefinition(float _secondsPerFrame) :
	secondsPerFrame(_secondsPerFrame)
{
}

SpriteSheetAnimationDefinition::~SpriteSheetAnimationDefinition(){
}

void SpriteSheetAnimationDefinition::pushFrame(unsigned long int _column, unsigned long int _row, float _width, float _height, float _textureWidth, float _textureHeight){
	float u, v, rW, rH;
	rW = /*1.f - (*/_width / _textureWidth/*)*/;
	rH = -_height / _textureHeight;
	u = rW * _column;
	v = 1.f + (rH * _row);
	frames.push_back(sweet::Rectangle(u, v, rW, rH));
}

void SpriteSheetAnimationDefinition::pushMultipleFrames(std::vector<unsigned long int> _frames, float _width, float _height, float _textureWidth, float _textureHeight){
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
		pushFrame(curCol - colOffset, curRow, _width, _height, _textureWidth, _textureHeight);
	}
}

void SpriteSheetAnimationDefinition::pushFramesInRange(unsigned long int _min, unsigned long int _max, float _width, float _height, float _textureWidth, float _textureHeight){
	unsigned long int curRow = 0;
	unsigned long int curCol = _min;
	
	unsigned long int range = _max - _min + 1;

	unsigned long int frames = 0;

	
	// while the current column is further than the texture's width,
	// go to the next row and move the current column back by a full texture unit
	while((curCol + 0.5f) * _width >= _textureWidth){
		++curRow;
		curCol -= _textureWidth/_width;
	}

	do{
		// while the current column is less than the texture's width,
		// add the current column as a frame and go to the next column
		while(curCol * _width <= _textureWidth){
			pushFrame(curCol, curRow, _width, _height, _textureWidth, _textureHeight);

			++frames;
			++curCol;

			if(frames >= range){
				break;
			}
		}
		++curRow; // go to the next row
		curCol -= _textureWidth/_width;
	}while(frames < range);
}

SpriteSheetAnimationInstance::SpriteSheetAnimationInstance(const SpriteSheetAnimationDefinition * const _definition) :
	definition(_definition),
	currentFrame(0),
	frameIndices(new Animation<unsigned long int>(&currentFrame))
{
	frameIndices->hasStart = true;

	for(auto f : definition->frames){
		frameIndices->tweens.push_back(new Tween<unsigned long int>(definition->secondsPerFrame, 1, Easing::kLINEAR));
	}
}

SpriteSheetAnimationInstance::~SpriteSheetAnimationInstance(){
	delete frameIndices;
}

void SpriteSheetAnimationInstance::update(Step* _step){
	if(definition->secondsPerFrame != 0){
		frameIndices->update(_step);
		currentFrame = frameIndices->currentTween;
	}
}