#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\ComponentShaderBase.h>
#include <BulletWorld.h>
#include <BulletDebugDrawer.h>

class UILayer : public Entity{
public:
	OrthographicCamera cam;
	ComponentShaderBase * shader;

	BulletWorld * bulletWorld;
	BulletDebugDrawer * bulletDebugDrawer;

	UILayer(float _left, float _right, float _bottom, float _top);
	~UILayer();
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _bottom, float _top);
	virtual void update(Step * _step) override;
	//bool addChild(NodeChild * _child) override;
};