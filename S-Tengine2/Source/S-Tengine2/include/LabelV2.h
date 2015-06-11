#pragma once

#include "NodeUI.h"

#define INFINITE_WIDTH -1

class Font;

enum WrapMode {
	CHARACTER_WRAP,
	CHARACTER_WRAP_HYPHEN,
	WORD_WRAP
};

enum Alignment {
	LEFT,
	CENTER
};


class GlyphMeshEntity : public MeshEntity {
public:
	float width;
	wchar_t character; 
	bool inUse;

	GlyphMeshEntity(MeshInterface* _mesh, Shader* _shader, float _width, wchar_t _character);
};

class Line : public Transform {
public:
	float width;
	bool inUse; 
	Line();
	void invalidate();
	void insertGlyph(GlyphMeshEntity * _glyph, Shader * _textShader);
};

class LabelV2 : public NodeUI{

public:
	
	Font * font;
	Shader * textShader;
	Shader * backgroundShader;
	float width;
	WrapMode wrapMode;

	LabelV2(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader, Shader * _backgroundShader, WrapMode _wrapMode, float _width = INFINITE_WIDTH);

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void invalidateAllLines();
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::wstring _text);
	void appendText(std::wstring _text);
	std::wstring getText();
	void updateAlignment();
	void setAlignment(Alignment _alignment);
	float getMeasuredWidth() override;
	float getMeasuredHeight() override;

private:
	std::wstring text;
	bool updateRequired;

	Line * currentLine();
};