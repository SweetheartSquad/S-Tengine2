#include "CharacterComponent.h"
#include "Texture.h"
#include "MeshInterface.h"

CharacterComponent::CharacterComponent(float _componentScale, float _width, float _height, Texture* _texture, Box2DWorld* _world, b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform) :
	Box2DSprite(_world, _bodyType, _defaultFixture, _shader, _transform),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	width(_width),
	height(_height),
	scale(_componentScale)
{
	mesh->pushTexture2D(_texture);
	body->SetUserData(this);
}

float CharacterComponent::getCorrectedHeight(){
	return height*std::abs(transform->getScaleVector().y)*scale*2.f;
}
float CharacterComponent::getCorrectedWidth(){
	return width*std::abs(transform->getScaleVector().x)*scale*2.f;
}

void CharacterComponent::createFixture(int16 _groupIndex){
	b2PolygonShape tShape;
	glm::vec3 sv = transform->getScaleVector();
	tShape.SetAsBox(width*std::abs(sv.x)*scale*2.f, height*std::abs(sv.y)*scale*2.f);

	b2Fixture * f = body->CreateFixture(&tShape, 1); // physical fixture

	// physical fixture
	b2Filter tf;
	tf.groupIndex = _groupIndex;
	f->SetFilterData(tf);

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
	
	float mag = std::max(mesh->getTexture(0)->width, mesh->getTexture(0)->height);
	mesh->vertices.at(3).u = 0;
	mesh->vertices.at(3).v = 0;
	mesh->vertices.at(2).u = width/mag;
	mesh->vertices.at(2).v = 0;
	mesh->vertices.at(1).u = width/mag;
	mesh->vertices.at(1).v = height/mag;
	mesh->vertices.at(0).u = 0;
	mesh->vertices.at(0).v = height/mag;
}