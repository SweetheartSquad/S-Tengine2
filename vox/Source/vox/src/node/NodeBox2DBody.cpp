#pragma once

#include "node/NodeBox2DBody.h"
#include "Box2DWorld.h"

NodeBox2DBody::NodeBox2DBody(Box2DWorld * _world, b2BodyType _bodyType, bool _defaultFixture, Transform* _transform):
	NodeTransformable(_transform),
	NodeUpdatable(),
	body(nullptr),
	defaultFixture(_defaultFixture),
	maxVelocity(b2Vec2(-1, -1)),
	prevAngle(0),
	world(_world)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;
	body = world->b2world->CreateBody(&bodyDef);
}

NodeBox2DBody::~NodeBox2DBody(){
	if(body != nullptr){
		world->b2world->DestroyBody(body);
	}
}

void NodeBox2DBody::update(Step * _step){
	if(body != nullptr){
		transform->translationVector.x = body->GetPosition().x;	
		transform->translationVector.y = body->GetPosition().y;	
		float lvx = body->GetLinearVelocity().x;
		float lvy = body->GetLinearVelocity().y;
		if(maxVelocity.x != -1 && abs(lvx) > abs(maxVelocity.x)){
			lvx = maxVelocity.x * (lvx < 0 ? -1 : 1);
		}
		if(maxVelocity.y != -1 && lvy > maxVelocity.y){
			lvy = maxVelocity.y * (lvy < 0 ? -1 : 1);
		}
		body->SetLinearVelocity(b2Vec2(lvx, lvy));
		if(abs(body->GetAngle() - prevAngle) > 0.00005f){
			transform->rotate(glm::degrees(body->GetAngle() - prevAngle), 0, 0, 1, kOBJECT);
			prevAngle = body->GetAngle();
		}
	}
}

void NodeBox2DBody::setTranslationPhysical(glm::vec3 _translation, bool _relative){
	if(_relative){
		transform->translate(_translation);
		if(body != nullptr){
			bodyDef.position.Set(bodyDef.position.x + _translation.x,bodyDef.position.y +  _translation.y);
		}
	}else{
		transform->translationVector = _translation;
		bodyDef.position.Set(transform->translationVector.x, transform->translationVector.y);
		if(body != nullptr){
			body->SetTransform(b2Vec2(_translation.x, _translation.y), body->GetAngle());
		}
	}
}

void NodeBox2DBody::setTranslationPhysical(float _x, float _y, float _z, bool _relative){
	setTranslationPhysical(glm::vec3(_x, _y, _z), _relative);
}

void NodeBox2DBody::setXPhysical(float _x){
	setTranslationPhysical(glm::vec3(_x, 0, 0));
}

void NodeBox2DBody::setYPhysical(float _y){
	setTranslationPhysical(glm::vec3(0, _y, 0));
}

void NodeBox2DBody::setXYPhysical(float _x, float _y){
	setTranslationPhysical(glm::vec3(_x, _y, 0));
}

void NodeBox2DBody::applyForce(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void NodeBox2DBody::applyForceLeft(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyForceRight(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyForceUp(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyForceDown(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyLinearImpulse(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void NodeBox2DBody::applyLinearImpulseLeft(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyLinearImpulseRight(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyLinearImpulseUp(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyLinearImpulseDown(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void NodeBox2DBody::applyAngularImpule(float _angle){
	if(body != nullptr){
		body->ApplyAngularImpulse(_angle, true);
	}
}

bool NodeBox2DBody::movingVertically(float _threshold){
	if(body != nullptr){
		return body->GetLinearVelocity().y > abs(_threshold) || body->GetLinearVelocity().y < _threshold * -1;
	}
	return false;
}

bool NodeBox2DBody::movingHorizontally(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold) || abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool NodeBox2DBody::movingRight(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold);
	}
	return false;
}

bool NodeBox2DBody::movingLeft(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool NodeBox2DBody::movingUp(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) > abs(_threshold);
	}
	return false;
}

bool NodeBox2DBody::movingDown(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) < abs(_threshold);
	}
	return false;
}