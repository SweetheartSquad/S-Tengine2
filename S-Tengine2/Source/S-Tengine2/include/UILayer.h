#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\ComponentShaderBase.h>
#include <BulletWorld.h>
#include <BulletDebugDrawer.h>
#include <NodeUI.h>

class UILayer : public virtual Entity, public virtual NodeUI{
public:
	OrthographicCamera cam;
	ComponentShaderBase * shader;

	BulletDebugDrawer * bulletDebugDrawer;

	UILayer(Scene * _scene, float _left, float _right, float _bottom, float _top);
	~UILayer();
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _bottom, float _top);
	virtual void update(Step * _step) override;
	
	//bool addChild(NodeChild * _child) override;
};