#pragma once

#include <Box2D/Box2D.h>

#include "shader/ComponentShaderBase.h"
#include "Sprite.h"
#include "node\NodeLoadable.h"

#define CIRCLE_VERTS 16

class Scene;
class Box2DWorld;

class Box2DDebugDrawer : public b2Draw, public virtual Entity, public virtual NodeShadable{
public:
	bool drawing;

	explicit Box2DDebugDrawer(Box2DWorld * _world);
	~Box2DDebugDrawer();
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	void load() override;
	void unload() override;

private:
	Sprite * spriteSegment, * spriteTransform, * spriteCircle, * spritePoly;
	Scene * scene;
	Box2DWorld * world;

	vox::MatrixStack * matrixStack;
	RenderOptions * renderOptions;
};