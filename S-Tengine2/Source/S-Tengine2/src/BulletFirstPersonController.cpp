#pragma once
#include <BulletFirstPersonController.h>

#include <node/NodeChild.h>
#include <node/NodeUpdatable.h>

#include <Scene.h>
#include <BulletDebugDrawer.h>
#include <BulletWorld.h>
#include <NodeBulletBody.h>

#include <Joystick.h>
#include <NodeBulletBody.h>
#include <NodeUI.h>
#include <MousePerspectiveCamera.h>
#include <Log.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <PointLight.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentMVP.h>

#include <MousePerspectiveCamera.h>

#include <RenderOptions.h>


#include <sweet/Input.h>

#include <Animation.h>
#include <Tween.h>

#include <OpenALSound.h>

#include <NumberUtils.h>
#include <Timeout.h>

BulletFirstPersonController::BulletFirstPersonController(BulletWorld * _bulletWorld, float _radius, float _height, float _mass, unsigned short int _collisionGroup, unsigned short int _collisionMask) : 
	NodeBulletBody(_bulletWorld),
	jumpTime(0.0),
	camOffset(0),
	// speed
	playerSpeed(0.5f),
	sprintSpeed(2.f),
	jumpSpeed(5.f),
	// collider
	playerRad(_radius),
	playerHeight(_height),
	mass(_mass),
	footSteps(nullptr),
	jumpSound(nullptr),
	landSound(nullptr),
	isSprinting(false),
	maxPitch(80),
	minPitch(-80),
	bobbleMultiplier(1)
{

	// player set-up
	setColliderAsCapsule(playerRad, playerHeight);
	createRigidBody(_mass, _collisionGroup, _collisionMask);
	body->setFriction(1);
	body->setAngularFactor(btVector3(0,0,0));
	body->setLinearFactor(btVector3(1,0.9,1));
	maxVelocity = btVector3(20,20,20);
	maxSpeedWalking = 12.0f;
	maxSpeedSprinting = 15.0f;
	maxSpeedCurrent = maxSpeedWalking;

	//Head Bobble Animation
	headBobble = new Animation<float>(&bobbleVal);
	headBobble->loopType = Animation<float>::LoopType::kLOOP;
	headBobbleTween1 = new Tween<float>(0.15f,-0.05f,Easing::kEASE_IN_OUT_CUBIC);
	headBobbleTween2 = new Tween<float>(0.15f,0.05f,Easing::kEASE_IN_OUT_CUBIC);
	headBobble->startValue = 0.f;
	headBobble->tweens.push_back(headBobbleTween1);
	headBobble->tweens.push_back(headBobbleTween2);
	headBobble->hasStart = true;

	currentBobbleTween = 0;
	lastBobbleTween = 0;
	tweenBobbleChange = false;

	headZoom = new Animation<float>(&zoomVal);
	//headZoom->loopType = Animation<float>::LoopType::kCONSTANT;
	headZoomTween1 = new Tween<float>(0.15f, 2.0f, Easing::kEASE_IN_OUT_CUBIC);
	headZoomTween2 = new Tween<float>(0.15f, 0.0f, Easing::kEASE_IN_OUT_CUBIC);
	headZoom->startValue = 1.f;
	headZoom->tweens.push_back(headZoomTween1);
	headZoom->tweens.push_back(headZoomTween2);
	headZoom->hasStart = true;

	//Default Interpolation, controls how extremem head bobble is
	bobbleInterpolation = 0.f;

	lastYVel = 0;
	
	MousePerspectiveCamera * c = new MousePerspectiveCamera();
	c->interpolation = 0.8f;
	c->farClip = 1000.f;
	c->nearClip = 0.1f;
	c->yaw = 90.0f;
	c->pitch = 0.0f;
	playerCamera = c;
	playerCameraController = c->controller;
};

BulletFirstPersonController::~BulletFirstPersonController(){
	if(footSteps != nullptr){
		footSteps->decrementAndDelete();
	}
	if(jumpSound != nullptr){
		jumpSound->decrementAndDelete();
	}
	if(landSound != nullptr){
		landSound->decrementAndDelete();
	}

	delete headBobble;
	delete headZoom;
}

void BulletFirstPersonController::update(Step * _step){
	BulletController::update(_step);
	NodeBulletBody::update(_step);
}

void BulletFirstPersonController::handleInputs(Step * _step, glm::vec3 _inputs){
	// parse inputs
	bool attemptJump = _inputs.y > FLT_EPSILON;
	glm::vec2 movement(_inputs.x, _inputs.z);



	// get player velocity
	glm::vec3 curVelocity = getLinearVelocity();
	

	currentBobbleTween = headBobble->currentTween;

	// detect when the animation loops over
	tweenBobbleChange = (currentBobbleTween != lastBobbleTween && currentBobbleTween == 1);

	//restrict how player can rotate head upward and downward around x-axis
	if(playerCamera->pitch > maxPitch){
		playerCamera->pitch = maxPitch;
	}else if(playerCamera->pitch < minPitch){
		playerCamera->pitch = minPitch;
	}

	currentYVel = curVelocity.y;
	if (currentYVel > 0.f && lastYVel < 0.f && isGrounded && jumpTime > 0.025f){
		if(landSound != nullptr){
			landSound->play();
		}
		jumpTime = 0.0;
	}

	lastYVel = currentYVel;

	// Movement speed multiplier while in the air
	float airControl = 0.1f;

	if(!isGrounded){
		movement *= airControl;
	}else{
		// jumping
		if(attemptJump){
			applyLinearImpulseToCenter(glm::vec3(0.f, jumpSpeed * mass, 0.f));
			if(jumpSound != nullptr){
				jumpSound->play();
			}
			jumpTime = _step->time;
		}
	}
	
	
	glm::vec2 glmCurVelocityXZ = glm::vec2(curVelocity.x, curVelocity.z);
	glm::vec2 normCurvelocityXZ(glm::normalize(glmCurVelocityXZ));
	glm::vec2 normMovementXZ(glm::normalize(movement));

	float movementMag = glm::length(movement);
	float glmCurVelocityMagXZ = glm::length(glmCurVelocityXZ);
	// If player is moving
	if(movementMag > FLT_EPSILON){
		if(glmCurVelocityMagXZ <= FLT_EPSILON){
			movement *= 10;
		}

		// randomize pitch of footsteps and play sound when the animation loops over
		if(tweenBobbleChange && glmCurVelocityMagXZ >= 1.0f){
			if(footSteps != nullptr){
				float pitchRand = sweet::NumberUtils::randomFloat(0.75f, 1.75f);
				footSteps->setPitch(pitchRand);
				footSteps->play();
			}
		}
		
		// recalculate movement speed based on intended playerSpeed and mass
		movement *= playerSpeed * mass;
		// if the player is running, multiply speed by a constant
		if(isSprinting && isGrounded){
			movement *= sprintSpeed;
			maxSpeedCurrent = maxSpeedSprinting;
		}else{
			maxSpeedCurrent = maxSpeedWalking;
		}

		body->activate(true);

		if(glmCurVelocityMagXZ < maxSpeedCurrent){
			if(bobbleInterpolation < 2.0f && glmCurVelocityMagXZ >= 1.0f){
				bobbleInterpolation += 0.1f;
			}else if(glmCurVelocityMagXZ < 1.0f){
				bobbleInterpolation = 0.f;
			}
			applyLinearImpulseToCenter(glm::vec3(movement.x, 0, movement.y));
		}
		

		// slows down the player when they're about to jump
		// (there's no friction once the player is off the ground, so we slow them down beforehand to compensate)
		/*if(keyboard->keyJustUp(GLFW_KEY_LEFT_SHIFT)||keyboard->keyJustDown(GLFW_KEY_SPACE)){
			if(glmCurVelocityMagXZ > FLT_EPSILON){
				body->applyCentralImpulse(btVector3(normCurvelocityXZ.x, 0, normCurvelocityXZ.y) * glmCurVelocityMagXZ * -0.3f);
			}
		}*/
		
		// if you aren't moving in the same direction as you are already going
		/*float d =glm::dot(normMovementXZ, normCurvelocityXZ);
		if(d < 0.7f){
			body->applyCentralImpulse(btVector3(curVelocity.x, 0, curVelocity.y) * -0.5f);
			//body->applyCentralImpulse(btVector3(movement.x, 0, movement.y) * 1.f);
		}*/
	}else{
		//If the player is not moving
		body->activate(true);
		//slow down body by applying force in opposite direction of its velocity
		glm::vec3 v = getLinearVelocity();
		applyLinearImpulseToCenter(glm::vec3(v.x, 0, v.z) * -0.5f);
		if(bobbleInterpolation > 0){
			bobbleInterpolation -= 0.1f;
		}else{
			bobbleInterpolation = 0.f;
		}
		
	}

	body->setDamping(0.5f,0.f);

	// If the player isnt moving vertically
	if(glm::abs(curVelocity.y) <= 0.01f){
		headBobble->update(_step);
	}

	lastBobbleTween = currentBobbleTween;

	//get player position
	const btVector3 & b = body->getWorldTransform().getOrigin();
	float rayRange = playerHeight * 0.9f;
	btVector3 rayEnd = b + btVector3(0,-1,0)*rayRange;
	btCollisionWorld::ClosestRayResultCallback GroundRayCallback(b, rayEnd);
	world->world->rayTest(b, rayEnd, GroundRayCallback);
	if(GroundRayCallback.hasHit()){
		//std::cout << "HIT" << std::endl;
		isGrounded = true;
	}else{
		isGrounded = false;
		jumpTime += _step->time - jumpTime;
	}
	playerCamera->firstParent()->translate(glm::vec3(b.x(), (playerHeight*0.75f+bobbleVal*bobbleInterpolation) * bobbleMultiplier + b.y(), b.z()) + camOffset, false);
}


glm::vec3 BulletFirstPersonController::getPosition() const{
	btVector3 playerPos = body->getWorldTransform().getOrigin();
	return glm::vec3(playerPos.x(), playerPos.y(), playerPos.z());
}
glm::vec3 BulletFirstPersonController::getLinearVelocity() const{
	btVector3 playerVel = body->getLinearVelocity(); 
	return glm::vec3(playerVel.x(), playerVel.y(), playerVel.z());
}

void BulletFirstPersonController::enable(){
	playerCameraController->movementEnabled = true;
	playerCameraController->rotationEnabled = true;
	playerCameraController->alignMouse();
	BulletController::enable();
}

void BulletFirstPersonController::disable(){
	playerCameraController->movementEnabled = false;
	playerCameraController->rotationEnabled = false;
	BulletController::disable();
}