#pragma once

#include "NodeUI.h"

#define INFINITE_WIDTH -1

class LabelV2;
class Font;
class Glyph;

class GlyphMeshEntity : public MeshEntity {
public:
	wchar_t character; 
	bool inUse;
	Glyph * glyph;

	GlyphMeshEntity(Glyph * _mesh, Shader* _shader, wchar_t _character);
	void setGlyphMesh(Glyph * _mesh);
};

class Line : public Transform {
public:

	LabelV2* label;
	float width;
	bool inUse;
	std::vector<NodeChild *> unusedGlyphs;

	explicit Line(LabelV2 * _label);
	
	void invalidate();
	void insertChar(wchar_t _char);
};

class LabelV2 : public NodeUI{

public:
	
	Font * font;
	Shader * textShader;
	Shader * backgroundShader;
	float width;
//	WrapMode wrapMode;
	Transform * lines;

	LabelV2(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader, Shader * _backgroundShader, float _width = INFINITE_WIDTH);

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void invalidateAllLines();
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::wstring _text);
	void appendText(std::wstring _text);
	std::wstring getText();
	void updateAlignment();
	//void setAlignment(Alignment _alignment);
	float getMeasuredWidth() override;
	float getMeasuredHeight() override;

private:
	std::wstring text;
	bool updateRequired;

	Line * currentLine();
};