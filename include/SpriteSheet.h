#pragma once

#include <vector>

#include "Rectangle.h"

class Texture;

class SpriteSheet{
public:

	std::vector<vox::Rectangle> frames;
	Texture * texture;

	explicit SpriteSheet(Texture * _texture);
	~SpriteSheet();

	void pushFrame(int _column, int _row, float _width, float _height);

};