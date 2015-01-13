#pragma once

#include "node/NodeBox2D.h"

NodeBox2D::NodeBox2D(b2BodyType _bodyType, bool _defaultFixture, Transform* _transform):
	NodeTransformable(_transform),
	NodeUpdatable(),
	body(nullptr),
	defaultFixture(_defaultFixture),
	maxVelocity(b2Vec2(-1, -1)),
	prevAngle(0)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;
}

NodeBox2D::~NodeBox2D(){
	if(body != nullptr){
		body->GetWorld()->DestroyBody(body);
	}
}

void NodeBox2D::update(Step * _step){
	if(body != nullptr){
		transform->translationVector.x = body->GetPosition().x;	
		transform->translationVector.y = body->GetPosition().y;	
		if(maxVelocity.x != -1 && abs(body->GetLinearVelocity().x) > abs(maxVelocity.x)){
			body->SetLinearVelocity(b2Vec2(maxVelocity.x * (body->GetLinearVelocity().x < 0 ? -1 : 1),  body->GetLinearVelocity().y));
		}
		if(maxVelocity.y != -1 && body->GetLinearVelocity().y > maxVelocity.y){
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, maxVelocity.y * (body->GetLinearVelocity().y < 0 ? -1 : 1)));
		}
		if(abs(body->GetAngle() - prevAngle) > 0.00005f){
			transform->rotate(glm::degrees(body->GetAngle() - prevAngle), 0, 0, 1, kOBJECT);
			prevAngle = body->GetAngle();
		}
	}
}

void NodeBox2D::setTranslationPhysical(glm::vec3 _translation){
	transform->translate(_translation);
	bodyDef.position.Set(transform->translationVector.x, transform->translationVector.y);
	if(body != nullptr){
		body->SetTransform(b2Vec2(_translation.x, _translation.y), body->GetAngle());
	}
}

void NodeBox2D::setTranslationPhysical(float _x, float _y, float _z){
	setTranslationPhysical(glm::vec3(_x, _y, _z));
}

void NodeBox2D::setXPhysical(float _x){
	setTranslationPhysical(glm::vec3(_x, 0, 0));
}

void NodeBox2D::setYPhysical(float _y){
	setTranslationPhysical(glm::vec3(0, _y, 0));
}

void NodeBox2D::setXYPhysical(float _x, float _y){
	setTranslationPhysical(glm::vec3(_x, _y, 0));
}

void NodeBox2D::applyForce(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void NodeBox2D::applyForceLeft(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyForceRight(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyForceUp(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyForceDown(float _force){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyLinearImpulse(float _forceX, float _forceY, float _pointX, float _pointY){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_forceX, _forceY), b2Vec2(_pointX, _pointY), true);
	}
}

void NodeBox2D::applyLinearImpulseLeft(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(-_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyLinearImpulseRight(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_force, 0), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyLinearImpulseUp(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, _force), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyLinearImpulseDown(float _force){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(0, -_force), body->GetWorldCenter(), true);
	}
}

void NodeBox2D::applyAngularImpule(float _angle){
	if(body != nullptr){
		body->ApplyAngularImpulse(_angle, true);
	}
}

bool NodeBox2D::movingVertically(float _threshold){
	if(body != nullptr){
		return body->GetLinearVelocity().y > abs(_threshold) || body->GetLinearVelocity().y < _threshold * -1;
	}
	return false;
}

bool NodeBox2D::movingHorizontally(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold) || abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool NodeBox2D::movingRight(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) > abs(_threshold);
	}
	return false;
}

bool NodeBox2D::movingLeft(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().x) < abs(_threshold);
	}
	return false;
}

bool NodeBox2D::movingUp(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) > abs(_threshold);
	}
	return false;
}

bool NodeBox2D::movingDown(float _threshold){
	if(body != nullptr){
		return abs(body->GetLinearVelocity().y) < abs(_threshold);
	}
	return false;
}