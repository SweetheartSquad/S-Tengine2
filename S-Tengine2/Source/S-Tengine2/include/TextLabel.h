#pragma once

#include "NodeUI.h"

#define UI_INFINITE -1

class TextArea;
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

class TextLabel : public NodeUI{
public:

	TextArea * textArea;
	Font * font;
	Shader * textShader;

	float lineWidth;
	bool inUse;
	std::vector<NodeChild *> unusedGlyphs;

	TextLabel(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader, float _width = UI_INFINITE);

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::wstring _text);
	void appendText(std::wstring _text);
	std::wstring getText();
	
	virtual void autoResizeHeight() override;
	virtual void autoResizeWidth() override;
	
	void invalidate();
	void insertChar(wchar_t _char);
	bool canFit(float _width);

private:

	std::wstring text;
	bool updateRequired;
};