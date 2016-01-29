#pragma once

#include <Box2DSprite.h>
#include <Box2DWorld.h>
#include <Texture.h>
#include <TextureSampler.h>
#include <MeshInterface.h>

Box2DSprite::Box2DSprite(Box2DWorld * _world, TextureSampler * _textureSampler, b2BodyType _bodyType, Shader* _shader, float _componentScale) :
	Sprite(_shader),
	NodeBox2DBody(_world, _bodyType),
	width(_textureSampler->width),
	height(_textureSampler->height),
	scale(_componentScale),
	u(_textureSampler->u),
	v(_textureSampler->v),
	useTextureSamplerUVs(true)
{
	if(_textureSampler != nullptr){
		mesh->pushTexture2D(_textureSampler->texture);
	}

	setUserData(this);
}
Box2DSprite::Box2DSprite(Box2DWorld * _world, b2BodyType _bodyType, Shader* _shader, Texture * _texture, float _width, float _height, float _u, float _v, float _componentScale):
	Sprite(_shader),
	NodeBox2DBody(_world, _bodyType),
	width(_width),
	height(_height),
	scale(_componentScale),
	u(_u),
	v(_v),
	useTextureSamplerUVs(false)
{
	if(_texture != nullptr){
		mesh->pushTexture2D(_texture);
	}

	setUserData(this);
}

float Box2DSprite::getCorrectedHeight(){
	glm::vec3 scaleVec(1);
	if(parents.size() > 0){
		scaleVec = childTransform->getScaleVector();
	}
	return height * std::abs(scaleVec.y)*scale;
}

float Box2DSprite::getCorrectedWidth(){
	glm::vec3 scaleVec(1);
	if(parents.size() > 0){
		scaleVec = childTransform->getScaleVector();
	}
	return width * std::abs(scaleVec.x)*scale;
}

b2Fixture * Box2DSprite::createFixture(b2Filter _filter, b2Vec2 _offset, void * _userData, bool _isSensor){
	b2PolygonShape tShape = createFixtureShape();

	b2FixtureDef fd;
	fd.shape = &tShape;
	fd.restitution = 0.f;
	fd.friction = 0.5f;
	fd.isSensor = _isSensor;
	fd.density = 1.f;
	fd.userData = _userData;
	fd.filter = _filter;

	b2Fixture * f = body->CreateFixture(&fd);

	// reposition mesh vertices to match collider vertices
	// NOTE: the vertex order is reversed between our mesh and Box2D's shapes
	const unsigned long int numVerts = tShape.GetVertexCount();
	for(unsigned long int i = 0; i < numVerts; ++i){
		mesh->vertices.at(i).x = tShape.GetVertex(numVerts-(i+1)).x;
		mesh->vertices.at(i).y = tShape.GetVertex(numVerts-(i+1)).y;
	}
	
	if(useTextureSamplerUVs){
		configureUVs();
	}

	return f;
}

void Box2DSprite::configureUVs(){
	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);
	setUvs(sweet::Rectangle(u/mag, v/mag, width/mag, height/mag));
}

b2PolygonShape Box2DSprite::createFixtureShape(){
	b2PolygonShape tShape;
	tShape.SetAsBox(getCorrectedWidth()*0.5f, getCorrectedHeight()*0.5f);
	return tShape;
}

Box2DSprite::~Box2DSprite(){
	if(world != nullptr && body != nullptr) {
		world->b2world->DestroyBody(body);
		body = nullptr;
	}
}

void Box2DSprite::update(Step * _step){
	NodeBox2DBody::update(_step);
	Sprite::update(_step);
}

void Box2DSprite::setGroupIndex(int16 _groupIndex){
	b2Fixture * f = body->GetFixtureList();
	while(f != nullptr){
		b2Filter bf = f->GetFilterData();
		bf.groupIndex = _groupIndex;
		f->SetFilterData(bf);
		f->Refilter(); // is this necessary?
		f = f->GetNext();
	}
}

void Box2DSprite::setUserData(void * _data){
	b2Fixture * f = body->GetFixtureList();
	while(f != nullptr){
		f->SetUserData(_data);
		f = f->GetNext();
	}
}