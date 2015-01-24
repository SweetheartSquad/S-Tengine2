#pragma once

#include "node/NodeRenderable.h"
#include "Texture.h"
#include "MeshInterface.h"
#include "Rectangle.h"

class BitmapFont : public QuadMesh
{
public:
	int asciiStart;
	int rows;
	int columns;
	std::vector<vox::Rectangle> frames;

	BitmapFont(Texture * _fontTextue, int _asciiStart, int _rows, int _columns);
	~BitmapFont();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void setText(std::string _text);
	void pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth);
	void pushFrame(int _column, int _row, float _width, float _height);
	void setSizeMod(float _mod);
private:
	std::string text;
	void createQuads();
	float modSize;
};