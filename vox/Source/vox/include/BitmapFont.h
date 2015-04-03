#pragma once

#include "node/NodeRenderable.h"
#include "MeshInterface.h"
#include "Rectangle.h"
#include "MeshEntity.h"

enum WrapMode {
	CHARACTER_WRAP,
	CHARACTER_WRAP_HYPHEN,
	WORD_WRAP
};

class BitmapFont : public MeshEntity
{
public:
	int asciiStart;
	int rows;
	float kerning;
	int columns;
	bool padFront;
	WrapMode wrapMode;

	QuadMesh * meshQ;
	std::vector<vox::Rectangle> frames;

	BitmapFont(Texture * _fontTextue, int _asciiStart, int _rows, int _columns, bool padFront = true, WrapMode _wrapMode = WORD_WRAP);
	~BitmapFont();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void setText(std::string _text);
	void pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth);
	void pushFrame(int _column, int _row, float _width, float _height);
	void setSizeMod(float _mod);
	void setWidthMod(float _mod);
	void renderText();

	float getWidth();
	float getHeight();
private:
	std::string text;
	void createQuads();
	float modSize;
	float width;
	float layoutWidth;
	float layoutHeight;
};