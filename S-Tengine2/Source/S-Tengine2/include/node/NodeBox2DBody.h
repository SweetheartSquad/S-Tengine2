#pragma once

#include <NodePhysicsBody.h>
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include <Transform.h>

#define NO_VELOCITY_LIMIT -1

class Box2DWorld;

// NOTE: physics bodies override Entity's update and don't call it
// This is because the MeshEntity varieties will also call it,
// causing problems in the most common use case. If you want the
// physics body to update its children, you have to extend this class
// and call it yourself in an overriden update call
class NodeBox2DBody : public virtual NodePhysicsBody{
public:
	Box2DWorld * world;

	b2Body * body;
	b2BodyDef bodyDef;

	//Max velocity. Should always be set as positive. -1 means no limit
	b2Vec2 maxVelocity;

	//bool defaultFixture;

	explicit NodeBox2DBody(Box2DWorld * _world, b2BodyType _bodyType = b2_dynamicBody);
	virtual ~NodeBox2DBody();	

	void update(Step* _step) override;

	//Creates a new fixture for the body and returns a reference to it
	b2Fixture * getNewFixture(b2PolygonShape _shape, float _density);
	b2Fixture * getNewFixture(b2ChainShape _shape, float _density);

	void applyForce(float _forceX, float _forceY, float _pointX, float _pointY);
	void applyForceLeft(float _force);
	void applyForceRight(float _force);
	void applyForceUp(float _force);
	void applyForceDown(float _force);

	void applyLinearImpulse(float _forceX, float _forceY, float _pointX, float _pointY);
	void applyLinearImpulseToCenter(float _forceX, float _forceY);
	void applyLinearImpulseLeft(float _force);
	void applyLinearImpulseRight(float _force);
	void applyLinearImpulseUp(float _force);
	void applyLinearImpulseDown(float _force);

	void applyAngularImpulse(float _angle);

	bool movingVertically(float _threshold = 0);
	bool movingHorizontally(float _threshold = 0);
	bool movingRight(float _threshold = 0);
	bool movingLeft(float _threshold = 0);
	bool movingUp(float _threshold = 0);
	bool movingDown(float _threshold = 0);
	
	// NOTE: z is ignored completely; translate the childTransform directly to affect the z translation
	virtual void translatePhysical(glm::vec3 _translation, bool _relative = true) override;
	virtual void realign() override;
private:
	float prevAngle;
};