#pragma once

#include "SpriteSheetAnimation.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Tween.h"
#include "Rectangle.h"

SpriteSheetAnimation::SpriteSheetAnimation(SpriteSheet * _spriteSheet, float _secondsPerFrame):
	NodeUpdatable(),
	frames(Animation<int>(&currentFrame)),
	currentFrame(0)
{
	frames.hasStart = true;
	for(unsigned long int i = 0; i < _spriteSheet->frames.size(); ++i){
		frames.tweens.push_back(new Tween<int>(_secondsPerFrame, 1, Easing::kNONE));
	}
}

SpriteSheetAnimation::~SpriteSheetAnimation(){
}

void SpriteSheetAnimation::update(Step* _step){
	frames.update(_step);
	currentFrame = frames.currentTween;
}