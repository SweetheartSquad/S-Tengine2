#pragma once

#include "node/NodeUpdatable.h"
#include "Animation.h"
#include "Rectangle.h"

class Texture;
class SpriteSheet;

class SpriteSheetAnimation : NodeUpdatable
{
public:
	Animation<unsigned long int> frameIndices;
	std::vector<vox::Rectangle> frames;
	Texture * texture;
	unsigned long int currentFrame;
	float secondsPerFrame;

	explicit SpriteSheetAnimation(Texture * _spriteSheet, float _secondsPerFrame);
	~SpriteSheetAnimation();

	void update(Step* _step) override;
	void pushFrame(unsigned long int _column, unsigned long int _row, float _width, float _height);
	
	void pushMultipleFrames(std::vector<unsigned long int> _frames, float _width, float _height);
	void pushFramesInRange(unsigned long int _min, unsigned long int _max, float _width, float _height);
	
	void pushMultipleFrames(std::vector<unsigned long int> _frames, float _width, float _height, float _textureWidth);
	void pushFramesInRange(unsigned long int _min, unsigned long int _max, float _width, float _height, float _textureWidth);
};