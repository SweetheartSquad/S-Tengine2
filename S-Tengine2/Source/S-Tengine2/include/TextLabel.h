#pragma once

#include <HorizontalLinearLayout.h>
#include <MeshEntity.h>
#include <WrapMode.h>

class TextArea;
class TextLabel;
class Font;
class GlyphMesh;

// NOTE - UIGLyphs do not have mouse interaction enabled by default
class UIGlyph : public virtual NodeUI, public virtual NodeShadable{
public:
	wchar_t character;
	GlyphMesh * glyph;

	UIGlyph(BulletWorld * _world, GlyphMesh * _mesh, Shader * _shader, wchar_t _character);
	void setGlyphMesh(GlyphMesh * _mesh);
	
	virtual void render(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
};

class TextLabel : public HorizontalLinearLayout{
public:
	Font * font;
	Shader * textShader;

	float lineWidth;
	std::vector<UIGlyph *> usedGlyphs;
	std::vector<UIGlyph *> unusedGlyphs;

	TextLabel(BulletWorld* _world, Font * _font, Shader * _textShader);
	
	virtual void render(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	virtual void update(Step * _step) override;
	virtual void unload() override;
	virtual void load() override;
	
	void setText(std::wstring _text);
	// converts _text to an std::wstring and calls setText(std::wstring)
	void setText(std::string _text);
	void updateText();
	
	virtual float getContentsHeight() override;
	virtual float getContentsWidth() override;
	
	void invalidate();
	void insertChar(wchar_t _char);
	bool canFit(float _width);
	
	std::wstring textDisplayed;
	std::wstring textOverflow;
	
	WrapMode wrapMode;

private:
	std::wstring textAll;
	bool updateRequired;

	UIGlyph * getGlyph(wchar_t _char, GlyphMesh * _glyphMesh);
};