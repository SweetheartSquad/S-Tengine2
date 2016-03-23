#pragma once

#include <HorizontalLinearLayout.h>
#include <MeshEntity.h>

class Font;
class Glyph;
class GlyphMesh;

enum WrapMode {
	kCHARACTER,
	kWORD,
	kNONE,
	kTRUNCATE
};

// NOTE - UIGLyphs do not have mouse interaction enabled by default
class UIGlyph : public virtual NodeUI, public virtual NodeShadable{
public:
	wchar_t character;
	GlyphMesh * glyphMesh;

	UIGlyph(BulletWorld * _world, Glyph * _mesh, Shader * _shader, wchar_t _character);
	virtual ~UIGlyph();

	void setGlyph(Glyph * _mesh);
	void setShader(Shader * _shader, bool _configureDefaultAttributes);

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
	virtual ~TextLabel();

	virtual void render(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	virtual void update(Step * _step) override;
	virtual void unload() override;
	virtual void load() override;

	void setText(std::wstring _text);
	// converts _text to an std::wstring and calls setText(std::wstring)
	void setText(std::string _text);
	std::wstring getText(bool _clipped);
	// if _clipped, returns textDisplayed
	// otherwise, returns textAll
	void updateText();
	void setShader(Shader * _shader, bool _configureDefaultAttributes);
	void setFont(Font * _font, bool _updateText = true);

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

	UIGlyph * getGlyph(wchar_t _char, Glyph * _glyph);

	unsigned long int wordWrap();
};