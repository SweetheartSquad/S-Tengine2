#pragma once 

#include <Entity.h>
#include "shader/Shader.h"

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

	explicit Label(Font * _font, Shader * _shader, WrapMode _wrapMode, float _width = INFINITE_WIDTH);
	~Label();
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::string _text);
	void appendText(std::string _text);
	std::string getText();
	void updateText();

private:
	std::string text;
	bool textDirty;
	void newLine(glm::vec2 * _offset);
	void updateChar(glm::vec2 * _offset, char _c);
};