#pragma once

#include "Character1.h"

#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

Character1::Character1(Box2DWorld * _world, bool _ai):
	Character(_world, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	Texture * torsoTex = new Texture("../assets/MoustacheTorso.png", 512, 512, true, true);     
	Texture * headTex = new Texture("../assets/MoustacheManHead.png", 512, 512, true, true);  		 
	Texture * upperArmTex = new Texture("../assets/MoustacheUA.png", 512, 512, true, true);  
	Texture * lowerArmTex = new Texture("../assets/MoustacheLA.png", 512, 512, true, true);  
	Texture * handTex = new Texture("../assets/MoustacheHand.png", 512, 512, true, true);  	 
	Texture * upperLegTex = new Texture("../assets/MoustacheUL.png", 512, 512, true, true);  
	Texture * lowerLegTex = new Texture("../assets/MoustacheLL.png", 512, 512, true, true); 

	torso	      = new CharacterComponent(componentScale, 160,	290,	torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, 160,	270,	headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, 55,	85,		handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, 55,	205,	upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, 55,	85,		handTex,_world, b2_dynamicBody, false);
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

	
	ratioY_shoulder_to_torso = 0.85f;
	ratioY_torso_to_shoulder = 0.3f;
	
	ratioY_hip_to_torso = 0.85f;
	ratioY_torso_to_hip = 0.85f;

	ratioY_knee_to_hip = 0.75f;
	ratioY_hip_to_knee = 0.75f;

	ratioY_torso_to_neck = 0.9f;
	ratioY_neck_to_torso = 0.9f;

    attachJoints();
}

Character1::~Character1(){
}