#pragma once

#include "Box2DSprite.h"

Box2DSprite::Box2DSprite(b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	body(nullptr),
	defaultFixture(_defaultFixture),
	maxVelocity(b2Vec2(-1, -1))
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
		if(maxVelocity.x != -1 && abs(body->GetLinearVelocity().x) > abs(maxVelocity.x)){
			body->SetLinearVelocity(b2Vec2(maxVelocity.x * (body->GetLinearVelocity().x < 0 ? -1 : 1),  body->GetLinearVelocity().y));
		}
		if(maxVelocity.y != -1 && body->GetLinearVelocity().y > maxVelocity.y){
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, maxVelocity.y * (body->GetLinearVelocity().y < 0 ? -1 : 1)));
		}
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

bool Box2DSprite::movingVertically(float _threshold){
	if(body != nullptr){
		return body->GetLinearVelocity().y > abs(_threshold) || body->GetLinearVelocity().y < _threshold * -1;
	}
	return false;
}

bool Box2DSprite::movingHorizontally(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold) || abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool Box2DSprite::movingRight(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold);
	}
	return false;
}

bool Box2DSprite::movingLeft(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool Box2DSprite::movingUp(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) > abs(_threshold);
	}
	return false;
}

bool Box2DSprite::movingDown(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) < abs(_threshold);
	}
	return false;
}