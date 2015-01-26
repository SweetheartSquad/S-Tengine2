#pragma once

#include "Character2.h"

#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

Character2::Character2(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits):
	Character(_world, _categoryBits, _maskBits, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	texture_packs character = kAFRO;
	torso	      = new CharacterComponent(componentScale, torsoTexPacks[character]->width,		torsoTexPacks[character]->height,		torsoTexPacks[character]->texture, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, headTexPacks[character]->width,		headTexPacks[character]->height,		headTexPacks[character]->texture, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture,_world, b2_dynamicBody, false);
	leftUpperLeg  = new CharacterComponent(componentScale, upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	leftLowerLeg  = new CharacterComponent(componentScale, lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightUpperLeg = new CharacterComponent(componentScale, upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	rightLowerLeg = new CharacterComponent(componentScale, lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->texture,_world, b2_dynamicBody, false);


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