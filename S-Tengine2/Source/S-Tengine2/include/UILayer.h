#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\ComponentShaderBase.h>
#include <BulletWorld.h>
#include <BulletDebugDrawer.h>
#include <NodeUI.h>

class Mouse;

class RayTestInfo{
public:
	btVector3 raystart;
	btVector3 raydir;
	btVector3 rayend;
	btTransform rayfrom;
	btTransform rayto;
	btCollisionWorld::AllHitsRayResultCallback raycb;
	RayTestInfo();
};

class UILayer : public NodeUI{
private:
	RayTestInfo rayInfo;
public:
	Mouse * mouse;
	OrthographicCamera cam;

	BulletDebugDrawer * bulletDebugDrawer;
	ComponentShaderBase * shader;

	UILayer(Scene * _scene, float _left, float _right, float _bottom, float _top);
	~UILayer();
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _bottom, float _top);
	virtual void update(Step * _step) override;

	void hitTest(NodeChild * _c);
};