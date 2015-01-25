#pragma once

#include "TestCharacter.h"
#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

TestCharacter::TestCharacter(Box2DWorld * _world):
	Character(_world),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	Texture * torsoTex = new Texture("../assets/MichaelTorso.png", 512, 512, true, true);     
	Texture * headTex = new Texture("../assets/MichaelHead.png", 512, 512, true, true);  		 
	Texture * upperArmTex = new Texture("../assets/MichaelUpperArm.png", 512, 512, true, true);  
	Texture * lowerArmTex = new Texture("../assets/MichaelLowerArm.png", 512, 512, true, true);  
	Texture * handTex = new Texture("../assets/MichaelHand.png", 512, 512, true, true);  	 
	Texture * upperLegTex = new Texture("../assets/MichaelUpperLeg.png", 512, 512, true, true);  
	Texture * lowerLegTex = new Texture("../assets/MichaelLowerLeg.png", 512, 512, true, true); 

	Texture * uvTex = new Texture("../assets/uv-test.jpg", 1000, 1000, true, true);

	torso	      = new CharacterComponent(componentScale, 150,	360,	torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, 160,	270,	headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, 50,	82,		handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, 50,	82,		handTex,_world, b2_dynamicBody, false);
	leftUpperLeg  = new CharacterComponent(componentScale, 70,	215,	upperLegTex,_world, b2_dynamicBody, false);
	leftLowerLeg  = new CharacterComponent(componentScale, 135,	285,	lowerLegTex,_world, b2_dynamicBody, false);
	rightUpperLeg = new CharacterComponent(componentScale, 70,	215,	upperLegTex,_world, b2_dynamicBody, false);
	rightLowerLeg = new CharacterComponent(componentScale, 135,	285,	lowerLegTex,_world, b2_dynamicBody, false);


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