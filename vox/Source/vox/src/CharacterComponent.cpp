#include "CharacterComponent.h"
#include "Character.h"
#include "Texture.h"

CharacterComponent::CharacterComponent(float _width, float _height, Texture* _texture, Box2DWorld* _world, b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform) :
	Box2DSprite(_world, _bodyType, _defaultFixture, _shader, _transform),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	width(_width),
	height(_height)
{
	mesh->pushTexture2D(_texture);
	
	float fullW = _texture->width;
	float fullH = _texture->height;
}

void CharacterComponent::createFixture(){
	float scale = dynamic_cast<Character *>(parent)->componentScale;
	b2PolygonShape tShape;
	tShape.SetAsBox(width*transform->scaleVector.x*scale*2.f, height*transform->scaleVector.y*scale*2.f);
	body->CreateFixture(&tShape, 1);
	
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

	mesh->vertices.at(0).x = v1.x/5.f;
	mesh->vertices.at(0).y = v1.y/5.f;
	mesh->vertices.at(1).x = v2.x/5.f;
	mesh->vertices.at(1).y = v2.y/5.f;
	mesh->vertices.at(2).x = v3.x/5.f;
	mesh->vertices.at(2).y = v3.y/5.f;
	mesh->vertices.at(3).x = v4.x/5.f;
	mesh->vertices.at(3).y = v4.y/5.f;
	
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