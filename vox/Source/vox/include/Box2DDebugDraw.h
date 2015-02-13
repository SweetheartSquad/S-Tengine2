#pragma once

#include <Box2D/Box2D.h>

#include "shader/BaseComponentShader.h"
#include "Sprite.h"
#include "node\NodeLoadable.h"

class Scene;
class Box2DWorld;

class Box2DDebugDraw : public b2Draw, public Entity{
public:
	bool drawing;

	explicit Box2DDebugDraw(Scene * _scene, Box2DWorld * _world);
	~Box2DDebugDraw();
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;

	virtual void load() override;
	virtual void unload() override;
	
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
private:
	BaseComponentShader * shader;
	Sprite * spriteSegment, * spriteTransform, * spriteCircle;
	Scene * scene;
	Box2DWorld * world;
};