#pragma once

#include <node/NodeBox2DBody.h>
#include <Box2DWorld.h>

NodeBox2DBody::NodeBox2DBody(Box2DWorld * _world, b2BodyType _bodyType) :
	body(nullptr),
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
	if(body != nullptr && (body->IsAwake() || directAdjustment)){
		realign();
	}
	NodePhysicsBody::update(_step);
}

void NodeBox2DBody::realign(){
	const b2Vec2 pos = body->GetPosition();
	childTransform->translate(pos.x, pos.y, childTransform->getTranslationVector().z, false);
			
	b2Vec2 lv = body->GetLinearVelocity();
	if(maxVelocity.x != -1 && abs(lv.x) > abs(maxVelocity.x)){
		lv.x = maxVelocity.x * (lv.x < 0 ? -1 : 1);
	}
	if(maxVelocity.y != -1 && abs(lv.y) > maxVelocity.y){
		lv.y = maxVelocity.y * (lv.y < 0 ? -1 : 1);
	}
	body->SetLinearVelocity(lv);
			
	if(abs(body->GetAngle() - prevAngle) > FLT_EPSILON){
		childTransform->rotate(glm::degrees(body->GetAngle() - prevAngle), 0, 0, 1, kOBJECT);
		prevAngle = body->GetAngle();
	}
	NodePhysicsBody::realign();
}

b2Fixture * NodeBox2DBody::getNewFixture(b2PolygonShape _shape, float _density){
	return body->CreateFixture(&_shape, _density);
}

b2Fixture * NodeBox2DBody::getNewFixture(b2ChainShape _shape, float _density){
	return body->CreateFixture(&_shape, _density);
}

void NodeBox2DBody::translatePhysical(glm::vec3 _translation, bool _relative){
	glm::vec3 tv = _relative ? childTransform->getTranslationVector() + _translation : _translation;
	if(body != nullptr){
		body->SetTransform(b2Vec2(tv.x, tv.y), body->GetAngle());
	}
	NodePhysicsBody::translatePhysical(_translation, _relative);
}

void NodeBox2DBody::applyLinearImpulse(glm::vec3 _impulse, glm::vec3 _point){
	if(body != nullptr){
		body->ApplyLinearImpulse(b2Vec2(_impulse.x, _impulse.y), b2Vec2(_point.x, _point.y), true);
	}
}

void NodeBox2DBody::applyForce(glm::vec3 _force, glm::vec3 _point){
	if(body != nullptr){
		body->ApplyForce(b2Vec2(_force.x, _force.y), b2Vec2(_point.x, _point.y), true);
	}
}

glm::vec3 NodeBox2DBody::getPhysicsBodyCenter(){
	if(body != nullptr){
		b2Vec2 v = body->GetWorldCenter();
		return glm::vec3(v.x, v.y, 0);
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