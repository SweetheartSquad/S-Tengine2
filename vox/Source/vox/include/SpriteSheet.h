#pragma once
#include <vector>

class Rectangle;
class Texture;

class SpriteSheet{
public:

	std::vector<Rectangle> frames;
	Texture * texture;

	explicit SpriteSheet(Texture * _texture);
	~SpriteSheet();

	void pushFrame(int _idx, float _width, float _height);

};