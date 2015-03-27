#pragma once

#include "TestCharacter.h"
#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"
#include "BitmapFont.h"
#include <TextureSampler.h>

TestCharacter::TestCharacter(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	GameJamCharacter(_world, _categoryBits, _maskBits, _groupIndex, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	texture_packs character = kMICHAEL;
	torso	      = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), torsoTexPacks[character]->texture,		torsoTexPacks[character]->width,	torsoTexPacks[character]->height,		torsoTexPacks[character]->u,	torsoTexPacks[character]->v,	componentScale);
	head		  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), headTexPacks[character]->texture,		headTexPacks[character]->width,		headTexPacks[character]->height,		headTexPacks[character]->u,		headTexPacks[character]->v,		componentScale);
	leftUpperArm  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperArmTexPacks[character]->texture,	upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->u,	upperArmTexPacks[character]->v,	componentScale);
	leftLowerArm  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerArmTexPacks[character]->texture,	lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->u,	lowerArmTexPacks[character]->v,	componentScale);
	leftHand	  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), handTexPacks[character]->texture,		handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->u,		handTexPacks[character]->v,		componentScale);
	rightUpperArm = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperArmTexPacks[character]->texture,	upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->u,	upperArmTexPacks[character]->v,	componentScale);
	rightLowerArm = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerArmTexPacks[character]->texture,	lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->u,	lowerArmTexPacks[character]->v,	componentScale);
	rightHand	  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), handTexPacks[character]->texture,		handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->u,		handTexPacks[character]->v,		componentScale);
	leftUpperLeg  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperLegTexPacks[character]->texture,	upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->u,	upperLegTexPacks[character]->v,	componentScale);
	leftLowerLeg  = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerLegTexPacks[character]->texture,	lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->u,	lowerLegTexPacks[character]->v,	componentScale);
	rightUpperLeg = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), upperLegTexPacks[character]->texture,	upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->u,	upperLegTexPacks[character]->v,	componentScale);
	rightLowerLeg = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), lowerLegTexPacks[character]->texture,	lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->u,	lowerLegTexPacks[character]->v,	componentScale);


	leftUpperLeg->transform->scale(-1, 1, 1);
	leftLowerLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);
	
	leftLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	leftUpperLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightUpperLeg->transform->scale(0.9f, 0.9f, 1.f);

	leftLowerArm->transform->scale(0.9f, 0.9f, 1.f);
	rightLowerArm->transform->scale(0.9f, 0.9f, 1.f);
	leftUpperArm->transform->scale(0.9f, 0.9f, 1.f);
	rightUpperArm->transform->scale(0.9f, 0.9f, 1.f);

    attachJoints();
}

TestCharacter::~TestCharacter(){
}