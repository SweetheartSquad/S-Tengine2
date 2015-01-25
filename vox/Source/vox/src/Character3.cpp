#pragma once

#include "Character3.h"

#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

Character3::Character3(Box2DWorld * _world, bool _ai):
	Character(_world, _ai),
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

	torso	      = new CharacterComponent(componentScale, 175,	285,	torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, 170,	300,	headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, 65,	165,	upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, 55,	165,	lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, 55,	85,		handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, 65,	165,	upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, 55,	165,	lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, 65,	85,		handTex,_world, b2_dynamicBody, false);
	leftUpperLeg  = new CharacterComponent(componentScale, 50,	205,	upperLegTex,_world, b2_dynamicBody, false);
	leftLowerLeg  = new CharacterComponent(componentScale, 115,	240,	lowerLegTex,_world, b2_dynamicBody, false);
	rightUpperLeg = new CharacterComponent(componentScale, 50,	205,	upperLegTex,_world, b2_dynamicBody, false);
	rightLowerLeg = new CharacterComponent(componentScale, 115,	240,	lowerLegTex,_world, b2_dynamicBody, false);


	leftUpperLeg->transform->scale(-1, 1, 1);
	leftLowerLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);

	
	leftLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightLowerLeg->transform->scale(0.9f, 0.9f, 1.f);
	leftUpperLeg->transform->scale(0.9f, 0.9f, 1.f);
	rightUpperLeg->transform->scale(0.9f, 0.9f, 1.f);

    attachJoints();
}

Character3::~Character3(){
}