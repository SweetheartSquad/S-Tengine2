#pragma once

#include "Box2DDebugDraw.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteMesh.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"

#include <GL/glew.h>
#include <Math.h>

Box2DDebugDraw::Box2DDebugDraw(Scene * _scene):
	shader(new BaseComponentShader()),
	scene(_scene),
	sprite(new Sprite())
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
}

Box2DDebugDraw::~Box2DDebugDraw(){
	delete sprite;
	delete shader;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
	//delete sprite->mesh;
	//sprite->mesh = new SpriteMesh(GL_STATIC_DRAW);
	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	sprite->setShader(shader, true);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	for(int32 i = 0; i < vertexCount; i++) {
		sprite->mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	sprite->mesh->dirty = true;
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color& color){
	//delete sprite->mesh;
	//sprite->mesh = new SpriteMesh(GL_STATIC_DRAW);
	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	sprite->setShader(shader, true);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	for(int32 i = 0; i < vertexCount; i++) {
		sprite->mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	sprite->mesh->dirty = true;
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){
	//delete sprite->mesh;
	//sprite->mesh = new SpriteMesh(GL_STATIC_DRAW);
	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	sprite->setShader(shader, true);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	for (int32 i = 0; i < 30; ++i){
		float percent = (i / static_cast<float>(30-1));
		float rad = percent * 2 * 3.14159f;
		float x = center.x + radius * cos(rad);
		float y = center.y + radius * sin(rad);
		sprite->mesh->pushVert(Vertex(x, y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	sprite->mesh->dirty = true;
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color){
	//delete sprite->mesh;
	//sprite->mesh = new SpriteMesh(GL_STATIC_DRAW);
	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	sprite->setShader(shader, true);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	for (int32 i = 0; i < 30; ++i){
		float percent = (i / static_cast<float>(30-1));
		float rad = percent * 2 * 3.14159f;
		float x = center.x + radius * cos(rad);
		float y = center.y + radius * sin(rad);
		sprite->mesh->pushVert(Vertex(x, y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	sprite->mesh->dirty = true;
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color){
	//delete sprite->mesh;
	//sprite->mesh = new SpriteMesh(GL_STATIC_DRAW);
	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	sprite->setShader(shader, true);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	sprite->mesh->pushVert(Vertex(p1.x, p1.y, 0.0001f, color.r, color.g, color.b, 1.f));
	sprite->mesh->pushVert(Vertex(p1.x, p1.y, 0.0001f, color.r, color.g, color.b, 1.f));
	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	sprite->mesh->dirty = true;
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf){
}


void Box2DDebugDraw::load(){
	shader->load();
	sprite->load();
}

void Box2DDebugDraw::unload(){
	shader->unload();
	sprite->unload();
}