#pragma once

#include "Character2.h"

#include "Box2DSprite.h"
#include "CharacterComponent.h"

Character2::Character2(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	GameJamCharacter(_world, _categoryBits, _maskBits, _groupIndex, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	texture_packs character = kAFRO;
	torso	      = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), torsoTexPacks[character]->width,		torsoTexPacks[character]->height,		torsoTexPacks[character]->texture, componentScale);
	head		  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), headTexPacks[character]->width,		headTexPacks[character]->height,		headTexPacks[character]->texture, componentScale);
	leftUpperArm  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture, componentScale);
	leftLowerArm  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture, componentScale);
	leftHand	  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture, componentScale);
	rightUpperArm = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture, componentScale);
	rightLowerArm = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture, componentScale);
	rightHand	  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture, componentScale);
	leftUpperLeg  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture, componentScale);
	leftLowerLeg  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->texture, componentScale);
	rightUpperLeg = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture, componentScale);
	rightLowerLeg = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->texture, componentScale);


	leftUpperLeg->transform->scale(-1, 1, 1);
	leftLowerLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);

	
	leftLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	leftUpperLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightUpperLeg->transform->scale(0.9f, 0.9f, 1.f);

	ratioY_knee_to_hip = 0.75f;
	ratioY_hip_to_knee = 0.75f;

    attachJoints();
}

Character2::~Character2(){
}