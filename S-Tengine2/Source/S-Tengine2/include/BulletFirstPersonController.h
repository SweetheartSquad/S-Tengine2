#pragma once

#include <NodeBulletBody.h>
#include <BulletController.h>

#include <MousePerspectiveCamera.h>
#include <Animation.h>
#include <Tween.h>
#include <OpenALSound.h>

class BulletFirstPersonController : public virtual BulletController, public virtual NodeBulletBody{
protected:
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

	float maxPitch, minPitch;

	bool isGrounded;
	bool isSprinting;

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

	float maxSpeedCurrent;
public:
	Camera * playerCamera;
	CameraController * playerCameraController;
	glm::vec3 camOffset;

	// base player speed
	float playerSpeed;
	// sprint multiplier (cumulative with playerSpeed)
	float sprintSpeed;
	// impulse applied upwards when jumping
	float jumpSpeed;
	float mass;
	float maxSpeedWalking;
	float maxSpeedSprinting;

	// how much the head bobble actually affects the camera
	float bobbleMultiplier;

	BulletFirstPersonController(BulletWorld * bulletWorld, float _radius, float _height, float _mass);
	~BulletFirstPersonController();

	glm::vec3 getLinearVelocity() const;
	glm::vec3 getPosition() const;

	virtual void update(Step * _step) override;

	virtual void enable() override;
	virtual void disable() override;

	virtual void handleInputs(Step * _step, glm::vec3 _inputs) override;
};