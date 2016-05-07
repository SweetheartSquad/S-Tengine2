#pragma once

#include "node/NodeUpdatable.h"
#include "Animation.h"
#include "Rectangle.h"
#include <node/NodeLoadable.h>

class Texture;
class SpriteSheet;
class SpriteSheetAnimationInstance;

class SpriteSheetAnimationDefinition : public Node{
public:
	std::vector<sweet::Rectangle> frames;
	float secondsPerFrame;

	explicit SpriteSheetAnimationDefinition(float _secondsPerFrame);
	~SpriteSheetAnimationDefinition();

	void pushFrame(unsigned long int _column, unsigned long int _row, float _width, float _height, float _textureWidth, float _textureHeight);
	
	void pushMultipleFrames(std::vector<unsigned long int> _frames, float _width, float _height, float _textureWidth, float _textureHeight);
	void pushFramesInRange(unsigned long int _min, unsigned long int _max, float _width, float _height, float _textureWidth, float _textureHeight);
};

class SpriteSheetAnimationInstance : public NodeUpdatable{
public:
	// reference to the animation definition which created this instance
	const SpriteSheetAnimationDefinition * const definition;

	unsigned long int currentFrame;
	Animation<unsigned long int> * frameIndices;

	explicit SpriteSheetAnimationInstance(const SpriteSheetAnimationDefinition * const _definition);
	~SpriteSheetAnimationInstance();

	void update(Step* _step) override;
};