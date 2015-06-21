#pragma once

#include <HorizontalLinearLayout.h>
#include <MeshEntity.h>

class TextArea;
class TextLabel;
class Font;
class Glyph;

class UIGlyph : public virtual NodeUI, public virtual MeshEntity {
public:
	wchar_t character;
	Glyph * glyph;

	UIGlyph(BulletWorld * _world, Scene * _scene, Glyph * _mesh, Shader * _shader, wchar_t _character);
	void setGlyphMesh(Glyph * _mesh);
	
	virtual void load() override;
	virtual void unload() override;
};

class TextLabel : public HorizontalLinearLayout{
public:
	Font * font;
	Shader * textShader;

	float lineWidth;
	std::vector<UIGlyph *> usedGlyphs;

	TextLabel(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader, float _width = -1);
	
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	virtual void update(Step * _step) override;
	virtual void unload() override;
	virtual void load() override;

	void setText(std::wstring _text);
	
	virtual float getContentsHeight() override;
	virtual float getContentsWidth() override;
	
	void invalidate();
	void insertChar(wchar_t _char);
	bool canFit(float _width);
	
	std::wstring textDisplayed;
	std::wstring textOverflow;
private:
	std::wstring textAll;
	bool updateRequired;
};