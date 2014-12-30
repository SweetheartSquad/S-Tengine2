#pragma once

#include "Box2DSprite.h"

Box2DSprite::Box2DSprite(b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	body(nullptr),
	defaultFixture(_defaultFixture)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;
}

Box2DSprite::~Box2DSprite(){
}

void Box2DSprite::update(Step * _step){
	if(body != nullptr){
		transform->translationVector.x = body->GetPosition().x;	
		transform->translationVector.y = body->GetPosition().y;	
	}
	Sprite::update(_step);
}

void Box2DSprite::setTranslationPhysical(glm::vec3 _translation){
	transform->translate(_translation);
	bodyDef.position.Set(transform->translationVector.x, transform->translationVector.y);
	if(body != nullptr){
		body->SetTransform(b2Vec2(_translation.x, _translation.y), body->GetAngle());
	}
}

void Box2DSprite::setTranslationPhysical(float _x, float _y, float _z){
	setTranslationPhysical(glm::vec3(_x, _y, _z));
}

void Box2DSprite::setXPhysical(float _x){
	setTranslationPhysical(glm::vec3(_x, 0, 0));
}

void Box2DSprite::setYPhysical(float _y){
	setTranslationPhysical(glm::vec3(0, _y, 0));
}

void Box2DSprite::setXYPhysical(float _x, float _y){
	setTranslationPhysical(glm::vec3(_x, _y, 0));
}

void Box2DSprite::applyForce(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void Box2DSprite::applyForceLeft(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyForceRight(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyForceUp(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyForceDown(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyLinearImpulse(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void Box2DSprite::applyLinearImpulseLeft(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyLinearImpulseRight(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyLinearImpulseUp(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyLinearImpulseDown(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void Box2DSprite::applyAngularImpule(float _angle){
	if(body != nullptr){
		body->ApplyAngularImpulse(_angle, true);
	}
}

