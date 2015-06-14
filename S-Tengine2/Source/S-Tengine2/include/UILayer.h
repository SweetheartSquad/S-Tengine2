#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\ComponentShaderBase.h>

class UILayer : public Entity{
public:
	OrthographicCamera cam;
	ComponentShaderBase * shader;

	UILayer(float _left, float _right, float _bottom, float _top);
	~UILayer();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _bottom, float _top);
	//bool addChild(NodeChild * _child) override;
};