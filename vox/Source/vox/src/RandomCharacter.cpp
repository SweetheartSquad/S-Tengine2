#pragma once

#include "RandomCharacter.h"
#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

RandomCharacter::RandomCharacter(Box2DWorld * _world, bool _ai):
	Character(_world, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	texture_packs character = (texture_packs)(std::rand()%torsoTexPacks.size());
	torso	      = new CharacterComponent(componentScale, torsoTexPacks[character]->width,		torsoTexPacks[character]->height,		torsoTexPacks[character]->texture, _world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%headTexPacks.size());
	head		  = new CharacterComponent(componentScale, headTexPacks[character]->width,		headTexPacks[character]->height,		headTexPacks[character]->texture, _world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%upperArmTexPacks.size());
	leftUpperArm  = new CharacterComponent(componentScale, upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%lowerArmTexPacks.size());
	leftLowerArm  = new CharacterComponent(componentScale, lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%handTexPacks.size());
	leftHand	  = new CharacterComponent(componentScale, handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%upperArmTexPacks.size());
	rightUpperArm = new CharacterComponent(componentScale, upperArmTexPacks[character]->width,	upperArmTexPacks[character]->height,	upperArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%lowerArmTexPacks.size());
	rightLowerArm = new CharacterComponent(componentScale, lowerArmTexPacks[character]->width,	lowerArmTexPacks[character]->height,	lowerArmTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%handTexPacks.size());
	rightHand	  = new CharacterComponent(componentScale, handTexPacks[character]->width,		handTexPacks[character]->height,		handTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%upperLegTexPacks.size());
	leftUpperLeg  = new CharacterComponent(componentScale, upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%lowerLegTexPacks.size());
	leftLowerLeg  = new CharacterComponent(componentScale, lowerLegTexPacks[character]->width,	lowerLegTexPacks[character]->height,	lowerLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%upperLegTexPacks.size());
	rightUpperLeg = new CharacterComponent(componentScale, upperLegTexPacks[character]->width,	upperLegTexPacks[character]->height,	upperLegTexPacks[character]->texture,_world, b2_dynamicBody, false);
	character = (texture_packs)(std::rand()%lowerLegTexPacks.size());
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

	leftLowerArm->transform->scale(0.9f, 0.9f, 1.f);
	rightLowerArm->transform->scale(0.9f, 0.9f, 1.f);
	leftUpperArm->transform->scale(0.9f, 0.9f, 1.f);
	rightUpperArm->transform->scale(0.9f, 0.9f, 1.f);

    attachJoints();
}

RandomCharacter::~RandomCharacter(){
}