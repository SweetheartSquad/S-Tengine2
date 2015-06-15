#pragma once

#include "NodeUI.h"

#define INFINITE_WIDTH -1

class TextLabel;
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

	TextLabel* label;
	float width;
	bool inUse;
	std::vector<NodeChild *> unusedGlyphs;

	explicit Line(TextLabel * _label);
	
	void invalidate();
	void insertChar(wchar_t _char);
	bool canFit(float _width);
};

class TextLabel : public NodeUI{

public:
	
	Font * font;
	Shader * textShader;
	std::vector<NodeChild *> unusedLines;
//	WrapMode wrapMode;
	Transform * lines;

	TextLabel(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader, float _width = INFINITE_WIDTH);

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

	Line * getLine();

	
	virtual void autoResizeHeight() override;
	virtual void autoResizeWidth() override;
private:
	std::wstring text;
	bool updateRequired;

	Line * currentLine();
};