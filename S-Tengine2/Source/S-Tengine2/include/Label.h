#pragma once 

#include <Entity.h>
#include <shader/Shader.h>
#include <map>

class MeshEntity;
class Font;

#define INFINITE_WIDTH -1

enum WrapMode {
	CHARACTER_WRAP,
	CHARACTER_WRAP_HYPHEN,
	WORD_WRAP
};

enum Alignment {
	LEFT,
	CENTER
};

class Label : public Entity{
public:
	
	Font * font;
	Shader * textShader;
	Shader * backgroundShader;
	float width;
	WrapMode wrapMode;
	std::map<wchar_t, MeshEntity *> meshEntities;

	explicit Label(Font * _font, Shader * _textShader, Shader * _backgroundShader, WrapMode _wrapMode, float _width = INFINITE_WIDTH);
	~Label();
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::wstring _text);
	void appendText(std::wstring _text);
	std::wstring getText();
	void updateText();
	void updateAlignment();
	void setAlignment(Alignment _alignment);

private:
	std::wstring text;
	std::wstring oldText;
	std::vector<glm::vec2> offsetCache;
	std::vector<float>lineWidths;
	float measuredWidth;
	float measuredHeight;
	bool textDirty;
	void newLine(glm::vec2 * _offset, int _idx);
	void updateChar(glm::vec2 * _offset, int _index, wchar_t _c);
	MeshEntity * background;
	int renderableGlyphs;
	Alignment alignment;
	std::wstring textInternal;
	bool wasAppended;
	int lineCount;
};