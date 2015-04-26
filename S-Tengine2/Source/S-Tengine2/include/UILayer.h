#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\BaseComponentShader.h>

class UILayer : public Entity{
public:
	OrthographicCamera cam;
	BaseComponentShader * shader;

	UILayer();
	~UILayer();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	bool addChild(NodeChild * _child) override;
};