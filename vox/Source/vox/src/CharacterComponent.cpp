#include "CharacterComponent.h"
#include "Character.h"
#include "Texture.h"

CharacterComponent::CharacterComponent(float _componentScale, float _width, float _height, Texture* _texture, Box2DWorld* _world, b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform) :
	Box2DSprite(_world, _bodyType, _defaultFixture, _shader, _transform),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	width(_width),
	height(_height),
	scale(_componentScale)
{
	mesh->pushTexture2D(_texture);
	
	float fullW = _texture->width;
	float fullH = _texture->height;
}

float CharacterComponent::getCorrectedHeight(){
	return height*std::abs(transform->scaleVector.y)*scale*2.f;
}
float CharacterComponent::getCorrectedWidth(){
	return width*std::abs(transform->scaleVector.x)*scale*2.f;
}

void CharacterComponent::createFixture(){
	b2PolygonShape tShape;
	tShape.SetAsBox(width*std::abs(transform->scaleVector.x)*scale*2.f, std::abs(height*transform->scaleVector.y)*scale*2.f);
	body->CreateFixture(&tShape, 1);
	b2Filter t;
	t.groupIndex = -8;
	body->GetFixtureList()->SetFilterData(t);

	b2Vec2 v1 = tShape.GetVertex(0);
	b2Vec2 v2 = tShape.GetVertex(1);
	b2Vec2 v3 = tShape.GetVertex(2);
	b2Vec2 v4 = tShape.GetVertex(3);


	/*mesh->vertices.at(0).x = (-width/2.f)*transform->scaleVector.x*scale;
	mesh->vertices.at(0).y = (-height/2.f)*transform->scaleVector.y*scale;
	mesh->vertices.at(1).x = (width/2.f)*transform->scaleVector.x*scale;
	mesh->vertices.at(1).y = (-height/2.f)*transform->scaleVector.y*scale;
	mesh->vertices.at(2).x = (width/2.f)*transform->scaleVector.x*scale;
	mesh->vertices.at(2).y = (height/2.f)*transform->scaleVector.y*scale;
	mesh->vertices.at(3).x = (-width/2.f)*transform->scaleVector.x*scale;
	mesh->vertices.at(3).y = (height/2.f)*transform->scaleVector.y*scale;*/

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