#pragma once

#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "Texture.h"
#include "MeshInterface.h"

Box2DSprite::Box2DSprite(Box2DWorld * _world, b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform, float _width, float _height, Texture * _texture, float _componentScale):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_world, _bodyType, _defaultFixture, _transform),
	width(_width),
	height(_height),
	scale(_componentScale)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;

	if(_texture != nullptr){
		mesh->pushTexture2D(_texture);
	}

	body->SetUserData(this);
}

float Box2DSprite::getCorrectedHeight(){
	return height*std::abs(transform->scaleVector.y)*scale*2.f;
}
float Box2DSprite::getCorrectedWidth(){
	return width*std::abs(transform->scaleVector.x)*scale*2.f;
}

void Box2DSprite::createFixture(b2Filter _filter){
	b2PolygonShape tShape;
	tShape.SetAsBox(width*std::abs(transform->scaleVector.x)*scale*2.f, std::abs(height*transform->scaleVector.y)*scale*2.f);

	b2Fixture * f = body->CreateFixture(&tShape, 1); // physical fixture

	// physical fixture
	f->SetFilterData(_filter);

	b2Vec2 v1 = tShape.GetVertex(0);
	b2Vec2 v2 = tShape.GetVertex(1);
	b2Vec2 v3 = tShape.GetVertex(2);
	b2Vec2 v4 = tShape.GetVertex(3);

	mesh->vertices.at(0).x = v1.x;
	mesh->vertices.at(0).y = v1.y;
	mesh->vertices.at(1).x = v2.x;
	mesh->vertices.at(1).y = v2.y;
	mesh->vertices.at(2).x = v3.x;
	mesh->vertices.at(2).y = v3.y;
	mesh->vertices.at(3).x = v4.x;
	mesh->vertices.at(3).y = v4.y;
	
	float mag = std::max(mesh->textures.at(0)->width, mesh->textures.at(0)->height);
	mesh->vertices.at(3).u = 0;
	mesh->vertices.at(3).v = 0;
	mesh->vertices.at(2).u = width/mag;
	mesh->vertices.at(2).v = 0;
	mesh->vertices.at(1).u = width/mag;
	mesh->vertices.at(1).v = height/mag;
	mesh->vertices.at(0).u = 0;
	mesh->vertices.at(0).v = height/mag;
}

b2PolygonShape Box2DSprite::getFixtureShape(){
	b2PolygonShape tShape;
	tShape.SetAsBox(width*std::abs(transform->scaleVector.x)*scale*2.f, std::abs(height*transform->scaleVector.y)*scale*2.f);
	return tShape;
}

Box2DSprite::~Box2DSprite(){
	if(world != nullptr) {
		world->b2world->DestroyBody(body);
	}
}

void Box2DSprite::update(Step * _step){
	NodeBox2DBody::update(_step);
	Sprite::update(_step);
}