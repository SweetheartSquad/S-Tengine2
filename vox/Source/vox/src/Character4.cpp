#pragma once

#include "Character4.h"

#include "Box2DSprite.h"
#include "Texture.h"
#include "CharacterComponent.h"

Character4::Character4(Box2DWorld * _world, bool _ai):
	Character(_world, _ai),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	Texture * torsoTex = new Texture("../assets/PonytailTorso.png", 512, 512, true, true);     
	Texture * headTex = new Texture("../assets/PonytailHead.png", 512, 512, true, true);  		 
	Texture * upperArmTex = new Texture("../assets/PonytailUA.png", 512, 512, true, true);  
	Texture * lowerArmTex = new Texture("../assets/PonytailLA.png", 512, 512, true, true);  
	Texture * handTex = new Texture("../assets/PonytailHand.png", 512, 512, true, true);  	 
	Texture * upperLegTex = new Texture("../assets/PonytailUL.png", 512, 512, true, true);  
	Texture * lowerLegTex = new Texture("../assets/PonytailLL.png", 512, 512, true, true); 

	torso	      = new CharacterComponent(componentScale, 160,	355,	torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(componentScale, 155,	365,	headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(componentScale, 60,	200,	upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(componentScale, 55,	160,	lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(componentScale, 60,	80,		handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(componentScale, 60,	200,	upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(componentScale, 40,	145,	lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(componentScale, 60,	80,		handTex,_world, b2_dynamicBody, false);
	leftUpperLeg  = new CharacterComponent(componentScale, 80,	185,	upperLegTex,_world, b2_dynamicBody, false);
	leftLowerLeg  = new CharacterComponent(componentScale, 110,	205,	lowerLegTex,_world, b2_dynamicBody, false);
	rightUpperLeg = new CharacterComponent(componentScale, 80,	185,	upperLegTex,_world, b2_dynamicBody, false);
	rightLowerLeg = new CharacterComponent(componentScale, 110,	205,	lowerLegTex,_world, b2_dynamicBody, false);


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

Character4::~Character4(){
}