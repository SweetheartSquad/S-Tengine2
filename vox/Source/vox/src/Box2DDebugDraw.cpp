#pragma once

#include "Box2DDebugDraw.h"
#include "shader/Box2DDebugShader.h"
#include "Scene2D.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"

#include <GL/glew.h>

Box2DDebugDraw::Box2DDebugDraw(Scene2D * _scene):
	shader(BaseComponentShader()),
	scene(_scene)
{
	shader.components.push_back(new TextureShaderComponent());
	shader.compileShader();
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
	Sprite sprite;
	sprite.setShader(&shader, true);
	sprite.mesh->vertices.clear();
	sprite.mesh->polygonalDrawMode = GL_POLYGON;
	for(unsigned long int i = 0; i < vertexCount; i++) {
		sprite.mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 0.3));
	}
	sprite.render(scene->matrixStack, scene->renderOptions);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color& color){
	Sprite sprite;
	sprite.setShader(&shader, true);
	sprite.mesh->vertices.clear();
	sprite.mesh->polygonalDrawMode = GL_POLYGON;
	for(unsigned long int i = 0; i < vertexCount; i++) {
		sprite.mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 0.3));
	}
	sprite.render(scene->matrixStack, scene->renderOptions);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color){
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color){
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf){
}