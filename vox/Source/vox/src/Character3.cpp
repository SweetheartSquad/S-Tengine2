#pragma once

#include "Character3.h"

#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

Character3::Character3(Box2DWorld * _world):
	Character(_world),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	Texture * torsoTex = new Texture("../assets/HoodieTorso.png", 512, 512, true, true);     
	Texture * headTex = new Texture("../assets/HoodieHead.png", 512, 512, true, true);  		 
	Texture * upperArmTex = new Texture("../assets/HoodieUA.png", 512, 512, true, true);  
	Texture * lowerArmTex = new Texture("../assets/HoodieLA.png", 512, 512, true, true);  
	Texture * handTex = new Texture("../assets/HoodieHand.png", 512, 512, true, true);  	 
	Texture * upperLegTex = new Texture("../assets/HoodieUL.png", 512, 512, true, true);  
	Texture * lowerLegTex = new Texture("../assets/HoodieLL.png", 512, 512, true, true); 

	torso	      = new CharacterComponent(componentScale, 150,	360,	torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, 160,	270,	headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, 55,	85,		handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, 55,	85,		handTex,_world, b2_dynamicBody, false);
	upperLeftLeg  = new CharacterComponent(componentScale, 70,	215,	upperLegTex,_world, b2_dynamicBody, false);
	lowerLeftLeg  = new CharacterComponent(componentScale, 135,	285,	lowerLegTex,_world, b2_dynamicBody, false);
	upperRightLeg = new CharacterComponent(componentScale, 70,	215,	upperLegTex,_world, b2_dynamicBody, false);
	lowerRightLeg = new CharacterComponent(componentScale, 135,	285,	lowerLegTex,_world, b2_dynamicBody, false);


	upperLeftLeg->transform->scale(-1, 1, 1);
	lowerLeftLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);

	
	lowerLeftLeg->transform->scale(0.9f, 0.9f, 1.f);
	lowerRightLeg->transform->scale(0.9f, 0.9f, 1.f);
	upperLeftLeg->transform->scale(0.9f, 0.9f, 1.f);
	upperRightLeg->transform->scale(0.9f, 0.9f, 1.f);

    attachJoints();
}

Character3::~Character3(){
}