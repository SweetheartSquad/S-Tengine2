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

class Label : public Entity{
public:
	
	Font * font;
	Shader * shader;
	float width;
	WrapMode wrapMode;
	std::map<wchar_t, MeshEntity *> meshEntities;

		explicit Label(Font * _font, Shader * _shader, WrapMode _wrapMode, float _width = INFINITE_WIDTH);
	~Label();
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::wstring _text);
	void appendText(std::wstring _text);
	std::wstring getText();
	void updateText();

private:
	std::wstring text;
	bool textDirty;
	void newLine(glm::vec2 * _offset);
	void updateChar(glm::vec2 * _offset, int _index, wchar_t _c);
};