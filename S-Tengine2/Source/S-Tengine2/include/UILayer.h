#pragma once

#include <Entity.h>
#include <OrthographicCamera.h>
#include <shader\ComponentShaderBase.h>
#include <BulletWorld.h>
#include <BulletDebugDrawer.h>
#include <NodeUI.h>

class Sprite;
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
	Mouse * mouse;
public:
	Sprite * mouseIndicator;
	OrthographicCamera cam;

	BulletDebugDrawer * bulletDebugDrawer;
	ComponentShaderBase * shader;

	UILayer(float _left, float _right, float _bottom, float _top);
	~UILayer();
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	void resize(float _left, float _right, float _bottom, float _top);
	virtual void update(Step * _step) override;

	void hitTest(NodeChild * _c);

	Sprite * addMouseIndicator();

	void load() override;
	void unload() override;
};