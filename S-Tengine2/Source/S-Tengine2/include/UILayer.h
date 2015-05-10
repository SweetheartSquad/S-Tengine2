#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\BaseComponentShader.h>

class UILayer : public Entity{
public:
	OrthographicCamera cam;
	BaseComponentShader * shader;

	UILayer(float _left, float _right, float _top, float _bottom);
	~UILayer();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _top, float _bottom);
	//bool addChild(NodeChild * _child) override;
};