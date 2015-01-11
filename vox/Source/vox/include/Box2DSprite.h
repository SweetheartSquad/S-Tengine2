#pragma once

#include "Sprite.h"
#include <Box2D/Box2D.h>

#define NO_VELOCITY_LIMIT -1

class Box2DSprite : public Sprite{
public:

	b2Body * body;
	b2BodyDef bodyDef;

	//Max velocity. Should always be set as positive. -1 means no limit
	b2Vec2 maxVelocity;

	bool defaultFixture;

	explicit Box2DSprite(b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform());
	~Box2DSprite();	

	void update(Step* _step) override;

	void setTranslationPhysical(glm::vec3 _translation);
	void setTranslationPhysical(float _x, float _y, float _z);
	void setXPhysical(float _x);
	void setYPhysical(float _y);
	void setXYPhysical(float _x, float _y);

	void applyForce(float _forceX, float _forceY, float _pointX, float _pointY);
	void applyForceLeft(float _force);
	void applyForceRight(float _force);
	void applyForceUp(float _force);
	void applyForceDown(float _force);

	void applyLinearImpulse(float _forceX, float _forceY, float _pointX, float _pointY);
	void applyLinearImpulseLeft(float _force);
	void applyLinearImpulseRight(float _force);
	void applyLinearImpulseUp(float _force);
	void applyLinearImpulseDown(float _force);

	void applyAngularImpule(float _angle);

	bool movingVertically(float _threshold = 0);
	bool movingHorizontally(float _threshold = 0);
	bool movingRight(float _threshold = 0);
	bool movingLeft(float _threshold = 0);
	bool movingUp(float _threshold = 0);
	bool movingDown(float _threshold = 0);

private:
	float prevAngle;
};