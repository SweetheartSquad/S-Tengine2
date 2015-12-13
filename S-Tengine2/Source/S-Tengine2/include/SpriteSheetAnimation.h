#pragma once

#include "node/NodeUpdatable.h"
#include "Animation.h"
#include "Rectangle.h"
#include <node/NodeLoadable.h>

class Texture;
class SpriteSheet;

class SpriteSheetAnimation : public NodeUpdatable{
public:
	Animation<unsigned long int> frameIndices;
	std::vector<sweet::Rectangle> frames;
	unsigned long int currentFrame;
	float secondsPerFrame;

	explicit SpriteSheetAnimation(float _secondsPerFrame);
	~SpriteSheetAnimation();

	void update(Step* _step) override;
	void pushFrame(unsigned long int _column, unsigned long int _row, float _width, float _height, float _textureWidth, float _textureHeight);
	
	void pushMultipleFrames(std::vector<unsigned long int> _frames, float _width, float _height, float _textureWidth, float _textureHeight);
	void pushFramesInRange(unsigned long int _min, unsigned long int _max, float _width, float _height, float _textureWidth, float _textureHeight);
};