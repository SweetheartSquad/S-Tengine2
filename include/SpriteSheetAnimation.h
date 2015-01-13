#pragma once

#include "node/NodeUpdatable.h"
#include "Animation.h"
#include "Rectangle.h"

class Texture;
class SpriteSheet;

class SpriteSheetAnimation : NodeUpdatable
{
public:
	Animation<int> frameIndicies;
	std::vector<vox::Rectangle> frames;
	Texture * texture;
	int currentFrame;
	float secondsPerFrame;

	explicit SpriteSheetAnimation(Texture * _spriteSheet, float _secondsPerFrame);
	~SpriteSheetAnimation();

	void update(Step* _step) override;
	void pushFrame(int _column, int _row, float _width, float _height);
	
	void pushMultipleFrames(std::vector<int> _frames, float _width, float _height);
	void pushFramesInRange(int _min, int _max, float _width, float _height);
	
	void pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth);
	void pushFramesInRange(int _min, int _max, float _width, float _height, float _textureWidth);
};