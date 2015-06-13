#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <node/NodeRenderable.h>
#include <node/NodeLoadable.h>
#include <Texture.h>
#include <shader/ComponentShaderBase.h>
#include <StandardFrameBuffer.h>
#include "MeshEntity.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"


class RenderSurface;

class Text : public NodeLoadable, public NodeRenderable {
public:

	FT_Face face;
	Texture * texture;
	Texture * textureT;
	bool textDirty;
	StandardFrameBuffer * frameBuffer;
	StandardFrameBuffer frameBuffer2;
	RenderSurface * renderSurface;
	MeshEntity * m;
	OrthographicCamera cam;

	explicit Text(std::string _fontSrc);
	~Text();

	void load() override;
	void unload() override;
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;

	void updateTexture();
	void setText(std::string _text);
	std::string getText();

private:
	std::string text;
	ComponentShaderBase * shader;
};