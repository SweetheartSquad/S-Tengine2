#pragma once 

#include <Entity.h>
#include "shader/Shader.h"

class MeshEntity;
class Font;

class Label : public Entity{
public:
	
	Font * font;
	Shader * shader;

	explicit Label(Font * _font, Shader * _shader);
	~Label();
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void setText(std::string _text);

private:
	std::string text;
	bool textDirty;
};