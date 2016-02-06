#pragma once

#include <node/NodeChild.h>
#include <node/NodeUpdatable.h>

#include <Scene.h>
#include <BulletDebugDrawer.h>
#include <BulletWorld.h>
#include <NodeBulletBody.h>
#include <MousePerspectiveCamera.h>

#include <Animation.h>
#include <Tween.h>

#include <OpenALSound.h>

class Keyboard;
class Mouse;
class Joystick;

class BulletFirstPersonController : public virtual NodeBulletBody{
protected:
	bool enabled;

	Keyboard * keyboard;
	Mouse * mouse;
	Joystick * joystick;

	float camYpos;

	Animation<float> * headBobble;
	Tween<float> * headBobbleTween1;
	Tween<float> * headBobbleTween2;
	float bobbleVal;
	float bobbleInterpolation;
	int currentBobbleTween;
	int lastBobbleTween;
	bool tweenBobbleChange;
	double jumpTime;

	bool isGrounded;

	Animation<float> * headZoom;
	Tween<float> * headZoomTween1;
	Tween<float> * headZoomTween2;
	float zoomVal;
	
	OpenAL_Sound * footSteps;
	
	OpenAL_Sound * jumpSound;

	OpenAL_Sound * landSound;

	float lastYVel;
	float currentYVel;

	const float playerHeight;
	const float playerRad;

	glm::vec3 lastPos;

	glm::vec3 startPos;
public:
	MousePerspectiveCamera * playerCamera;
	glm::vec3 camOffset;

	// base player speed
	float playerSpeed;
	// sprint multiplier (cumulative with playerSpeed)
	float sprintSpeed;
	// impulse applied upwards when jumping
	float jumpSpeed;
	float mass;
	float maxSpeed;

	BulletFirstPersonController(BulletWorld * bulletWorld, float _radius, float _height, float _mass);
	~BulletFirstPersonController();

	glm::vec3 getLinearVelocity() const;
	glm::vec3 getPosition() const;

	virtual void update(Step * _step) override;

	void enable();
	void disable();
	bool isEnabled();
};