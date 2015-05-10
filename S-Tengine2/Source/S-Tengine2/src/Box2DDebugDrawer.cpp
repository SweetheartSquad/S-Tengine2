#pragma once

#include "Box2DDebugDrawer.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteMesh.h"
#include "shader/BaseComponentShader.h"
#include "shader/ShaderComponentTexture.h"

#include "Box2DWorld.h"

#include "MatrixStack.h"

#include <GL/glew.h>

Box2DDebugDrawer::Box2DDebugDrawer(Box2DWorld * _world) :
	shader(new BaseComponentShader(false)),
	world(_world),
	spriteSegment(new Sprite()),
	spriteTransform(new Sprite()),
	spriteCircle(new Sprite()),
	spritePoly(new Sprite()),
	drawing(false),
	matrixStack(nullptr),
	renderOptions(nullptr)
{
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();
	
	spriteSegment->mesh->vertices.clear();
	spriteSegment->mesh->indices.clear();
	spriteSegment->mesh->polygonalDrawMode = GL_LINES;
	spriteSegment->mesh->pushVert(Vertex(0, 0, 0.0001f, 0, 0, 1.f, 1.f));
	spriteSegment->mesh->pushVert(Vertex(1, 0, 0.0001f, 0, 0, 1.f, 1.f));

	
	spriteTransform->mesh->vertices.clear();
	spriteTransform->mesh->indices.clear();
	spriteTransform->mesh->polygonalDrawMode = GL_LINES;
	spriteTransform->mesh->pushVert(Vertex(0, 0, 0.0001f, 255, 0, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(1, 0, 0.0001f, 255, 0, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(0, 0, 0.0001f, 0, 255, 0, 1.f));
	spriteTransform->mesh->pushVert(Vertex(0, 1, 0.0001f, 0, 255, 0, 1.f));

	
	spriteCircle->mesh->polygonalDrawMode = GL_LINE_STRIP;
	spriteCircle->mesh->vertices.clear();
	spriteCircle->mesh->indices.clear();
	for (int32 i = 0; i < CIRCLE_VERTS; ++i){
		float percent = (i / static_cast<float>(CIRCLE_VERTS-1));
		float rad = percent * 2 * 3.14159f;
		float x = cos(rad);
		float y = sin(rad);
		spriteCircle->mesh->pushVert(Vertex(x, y, 0.0001f, 0, 1.f, 1.f, 1.f));
	}

	spritePoly->mesh->polygonalDrawMode = GL_LINE_STRIP;
	spritePoly->mesh->vertices.clear();
	spritePoly->mesh->indices.clear();
	spritePoly->mesh->pushVert(Vertex(0.f, 0.f, 0.0001f, 0.8f, 0.5f, 0.6f, 1.f));
	spritePoly->mesh->pushVert(Vertex(1.f, 0.f, 0.0001f, 0.8f, 0.5f, 0.6f, 1.f));
	spritePoly->mesh->pushVert(Vertex(1.f, 1.f, 0.0001f, 0.8f, 0.5f, 0.6f, 1.f));
	spritePoly->mesh->pushVert(Vertex(0.f, 1.f, 0.0001f, 0.8f, 0.5f, 0.6f, 1.f));
	spritePoly->mesh->indices.push_back(0); // close the loop
	
	spriteSegment->setShader(shader, true);
	spriteTransform->setShader(shader, true);
	spriteCircle->setShader(shader, true);
	spritePoly->setShader(shader, true);

	childTransform->addChild(spriteSegment);
	childTransform->addChild(spriteTransform);
	childTransform->addChild(spriteCircle);
	childTransform->addChild(spritePoly);
}

Box2DDebugDrawer::~Box2DDebugDrawer(){
	delete spriteSegment;
	delete spriteTransform;
	delete spriteCircle;
	delete spritePoly;
	shader->decrementAndDelete();
}

void Box2DDebugDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
	if(vertexCount == 4){
		spritePoly->parents.at(0)->reset();
		spritePoly->parents.at(0)->translate(vertices[0].x, vertices[0].y, 0.0001f, false);
		b2Vec2 s1(vertices[1].y - vertices[0].y, vertices[1].x - vertices[0].x);
		b2Vec2 s2(vertices[3].y - vertices[0].y, vertices[3].x - vertices[0].x);
		spritePoly->parents.at(0)->rotate(glm::degrees(atan2(s1.x, s1.y)), 0, 0, 1, kOBJECT);
		spritePoly->parents.at(0)->scale(s1.Length(), s2.Length(), 1, false);
	}

	spritePoly->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color& color){
	if(vertexCount == 4){
		spritePoly->parents.at(0)->reset();
		spritePoly->parents.at(0)->translate(vertices[0].x, vertices[0].y, 0.0001f, false);
		b2Vec2 s1(vertices[1].y - vertices[0].y, vertices[1].x - vertices[0].x);
		b2Vec2 s2(vertices[3].y - vertices[0].y, vertices[3].x - vertices[0].x);
		spritePoly->parents.at(0)->rotate(glm::degrees(atan2(s1.x, s1.y)), 0, 0, 1, kOBJECT);
		spritePoly->parents.at(0)->scale(s1.Length(), s2.Length(), 1, false);
	}

	spritePoly->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){
	spriteCircle->parents.at(0)->translate(glm::vec3(center.x, center.y, 0), false);
	spriteCircle->parents.at(0)->scale(glm::vec3(radius, radius, radius), false);
	spriteCircle->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color){
	spriteCircle->parents.at(0)->translate(glm::vec3(center.x, center.y, 0), false);
	spriteCircle->parents.at(0)->scale(glm::vec3(radius, radius, radius), false);
	spriteCircle->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color){
	spriteSegment->parents.at(0)->translate(glm::vec3(p1.x, p1.y, 0), false);
	float x = p2.x - p1.x;
	float y = p2.y - p1.y;
	float mag = sqrt(x*x + y*y);
	spriteSegment->parents.at(0)->setOrientation(glm::quat(glm::angleAxis(glm::degrees(atan2(y, x)), glm::vec3(0, 0, 1))));
	spriteSegment->parents.at(0)->scale(glm::vec3(mag, mag, mag), false);
	spriteSegment->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::DrawTransform(const b2Transform& xf){
	spriteTransform->parents.at(0)->translate(xf.p.x, xf.p.y, 0, false);
	spriteTransform->parents.at(0)->setOrientation(glm::quat(glm::angleAxis(glm::degrees(xf.q.GetAngle()), glm::vec3(0, 0, 1))));
	spriteTransform->parents.at(0)->render(matrixStack, renderOptions);
}

void Box2DDebugDrawer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// save previous line width state and change
	GLfloat oldWidth;
	glGetFloatv(GL_LINE_WIDTH, &oldWidth);
	glLineWidth(2.5f);

	// save previous depth-test state and disable
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}

	if(drawing){
		matrixStack = _matrixStack;
		renderOptions = _renderOptions;
		world->b2world->DrawDebugData();
		matrixStack = nullptr;
		renderOptions = nullptr;
	}

	// restore previous depth-test state
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}

	// restore previous line width state
	glLineWidth(oldWidth);
}

void Box2DDebugDrawer::load(){
	spriteTransform->load();
	spriteCircle->load();
	spritePoly->load();
	spriteSegment->load();
}

void Box2DDebugDrawer::unload(){
	spriteTransform->unload();
	spriteCircle->unload();
	spritePoly->unload();
	spriteSegment->unload();
}