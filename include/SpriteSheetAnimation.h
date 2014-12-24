#pragma once

#include "node/NodeUpdatable.h"
#include "Animation.h"

class SpriteSheet;

class SpriteSheetAnimation : NodeUpdatable
{
public:
	
	Animation<int> frames;
	int currentFrame;

	explicit SpriteSheetAnimation(SpriteSheet * _spriteSheet, float _fps);
	~SpriteSheetAnimation();

	void update(Step* _step) override;
};