#pragma once

#include "Box2DDebugDraw.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteMesh.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"

#include "GameJamScene.h"
#include "Box2DWorld.h"

#include "MatrixStack.h"

#include <GL/glew.h>

Box2DDebugDraw::Box2DDebugDraw(Scene * _scene, Box2DWorld * _world):
	shader(new BaseComponentShader()),
	scene(_scene),
	world(_world),
	sprite(new Sprite()),
	spriteSegment(new Sprite()),
	spriteTransform(new Sprite()),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	shader->components.push_back(new TextureShaderComponent());
	shader->compileShader();
	
	sprite->setShader(shader, true);
	spriteSegment->setShader(shader, true);
	spriteTransform->setShader(shader, true);
	
	spriteSegment->mesh->vertices.clear();
	spriteSegment->mesh->indices.clear();
	spriteSegment->mesh->polygonalDrawMode = GL_LINES;
	spriteSegment->mesh->vertices.clear();
	spriteSegment->mesh->indices.clear();
	spriteSegment->mesh->pushVert(Vertex(0, 0, 0.0001f, 0, 0, 0, 1.f));
	spriteSegment->mesh->pushVert(Vertex(0, 0, 0.0001f, 0, 0, 0, 1.f));

	
	spriteTransform->mesh->vertices.clear();
	spriteTransform->mesh->indices.clear();
	spriteTransform->mesh->polygonalDrawMode = GL_LINES;
	spriteTransform->mesh->pushVert(Vertex(0, 0, 0.0001f, 255, 0, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(1, 0, 0.0001f, 255, 0, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(0, 0, 0.0001f, 0, 255, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(0, 1, 0.0001f, 0, 255, 0, 1.f));
}

Box2DDebugDraw::~Box2DDebugDraw(){
	delete sprite;
	delete spriteSegment;
	delete spriteTransform;
	delete shader;
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	sprite->mesh->polygonalDrawMode = GL_POLYGON;

	sprite->transform->translationVector = glm::vec3(0, 0, 0);

	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	for(int32 i = 0; i < vertexCount; i++) {
		sprite->mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color& color){
	sprite->mesh->polygonalDrawMode = GL_POLYGON;

	sprite->transform->translationVector = glm::vec3(0, 0, 0);

	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	for(int32 i = 0; i < vertexCount; i++) {
		sprite->mesh->pushVert(Vertex(vertices[i].x, vertices[i].y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	sprite->render(scene->matrixStack, scene->renderOptions);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	
	sprite->transform->translationVector = glm::vec3(center.x, center.y, 0);

	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	for (int32 i = 0; i < 30; ++i){
		float percent = (i / static_cast<float>(30-1));
		float rad = percent * 2 * 3.14159f;
		float x = radius * cos(rad);
		float y = radius * sin(rad);
		sprite->mesh->pushVert(Vertex(x, y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	sprite->mesh->polygonalDrawMode = GL_POLYGON;
	
	sprite->transform->translationVector = glm::vec3(center.x, center.y, 0);

	sprite->mesh->vertices.clear();
	sprite->mesh->indices.clear();
	for (int32 i = 0; i < 30; ++i){
		float percent = (i / static_cast<float>(30-1));
		float rad = percent * 2 * 3.14159f;
		float x = radius * cos(rad);
		float y = radius * sin(rad);
		sprite->mesh->pushVert(Vertex(x, y, 0.0001f, color.r, color.g, color.b, 1.f));
	}
	
	sprite->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	spriteSegment->transform->translationVector = glm::vec3(0, 0, 0);
	spriteSegment->mesh->vertices.at(0).x = p1.x;
	spriteSegment->mesh->vertices.at(0).y = p1.y;
	spriteSegment->mesh->vertices.at(0).red = color.r;
	spriteSegment->mesh->vertices.at(0).green = color.g;
	spriteSegment->mesh->vertices.at(0).blue = color.b;
	spriteSegment->mesh->vertices.at(1).x = p2.x;
	spriteSegment->mesh->vertices.at(1).y = p2.y;
	spriteSegment->mesh->vertices.at(1).red = color.r;
	spriteSegment->mesh->vertices.at(1).green = color.g;
	spriteSegment->mesh->vertices.at(1).blue = color.b;
	spriteSegment->mesh->dirty = true;
	spriteSegment->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf){
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	spriteTransform->transform->translationVector = glm::vec3(xf.p.x, xf.p.y, 0);
	spriteTransform->render(scene->matrixStack, scene->renderOptions);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}


void Box2DDebugDraw::load(){
	if(!loaded){
		sprite->load();
		spriteSegment->load();
		spriteTransform->load();
		shader->load();
	}

	// sprite is still loaded by the time you go fullscreen and reload the scene
	
	NodeLoadable::load();
}

void Box2DDebugDraw::unload(){
	if(loaded){
		sprite->unload();
		spriteSegment->unload();
		spriteTransform->unload();
		shader->unload();
	}
	
	NodeLoadable::unload();
}

void Box2DDebugDraw::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	world->b2world->DrawDebugData();
}