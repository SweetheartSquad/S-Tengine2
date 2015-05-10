#pragma once

#include <node/NodeBox2DBody.h>
#include <Box2DWorld.h>

NodeBox2DBody::NodeBox2DBody(Box2DWorld * _world, b2BodyType _bodyType, bool _defaultFixture) :
	body(nullptr),
	defaultFixture(_defaultFixture),
	maxVelocity(b2Vec2(-1, -1)),
	prevAngle(0),
	world(_world)
{
	bodyDef.position.Set(0, 0);
	bodyDef.type = _bodyType;
	body = world->b2world->CreateBody(&bodyDef);
}

NodeBox2DBody::~NodeBox2DBody(){
	if(world != nullptr && body != nullptr) {
		world->b2world->DestroyBody(body);
		body = nullptr;
		world = nullptr;
	}
}

void NodeBox2DBody::update(Step * _step){
	if(body != nullptr){
		if(body->IsAwake()){
			parents.at(0)->transform->translate(body->GetPosition().x, body->GetPosition().y, parents.at(0)->transform->getTranslationVector().z, false);
			
			b2Vec2 lv = body->GetLinearVelocity();
			if(maxVelocity.x != -1 && abs(lv.x) > abs(maxVelocity.x)){
				lv.x = maxVelocity.x * (lv.x < 0 ? -1 : 1);
			}
			if(maxVelocity.y != -1 && lv.y > maxVelocity.y){
				lv.y = maxVelocity.y * (lv.y < 0 ? -1 : 1);
			}
			body->SetLinearVelocity(lv);
			
			if(abs(body->GetAngle() - prevAngle) > 0.00005f){
				parents.at(0)->transform->rotate(glm::degrees(body->GetAngle() - prevAngle), 0, 0, 1, kOBJECT);
				prevAngle = body->GetAngle();
			}
		}
	}
}

b2Fixture * NodeBox2DBody::getNewFixture(b2PolygonShape _shape, float _density){
	return body->CreateFixture(&_shape, _density);
}

b2Fixture * NodeBox2DBody::getNewFixture(b2ChainShape _shape, float _density){
	return body->CreateFixture(&_shape, _density);
}

void NodeBox2DBody::setTranslationPhysical(glm::vec3 _translation, bool _relative){
	parents.at(0)->transform->translate(_translation, _relative);
	glm::vec3 tv = parents.at(0)->transform->getTranslationVector();
	if(body != nullptr){
		body->SetTransform(b2Vec2(tv.x, tv.y), body->GetAngle());
	}
}

void NodeBox2DBody::setTranslationPhysical(float _x, float _y, float _z, bool _relative){
	setTranslationPhysical(glm::vec3(_x, _y, _z), _relative);
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

void NodeBox2DBody::applyAngularImpulse(float _angle){
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