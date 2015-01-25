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

	torso	      = new CharacterComponent(150, 360, torsoTex, _world, b2_dynamicBody, false);
	head		  = new CharacterComponent(160, 270, headTex, _world, b2_dynamicBody, false);
	leftUpperArm  = new CharacterComponent(0,0,upperArmTex,_world, b2_dynamicBody, false);
	leftLowerArm  = new CharacterComponent(0,0,lowerArmTex,_world, b2_dynamicBody, false);
	leftHand	  = new CharacterComponent(0,0,handTex,_world, b2_dynamicBody, false);
	rightUpperArm = new CharacterComponent(0,0,upperArmTex,_world, b2_dynamicBody, false);
	rightLowerArm = new CharacterComponent(0,0,lowerArmTex,_world, b2_dynamicBody, false);
	rightHand	  = new CharacterComponent(0,0,handTex,_world, b2_dynamicBody, false);
	upperLeftLeg  = new CharacterComponent(0,0,upperLegTex,_world, b2_dynamicBody, false);
	lowerLeftLeg  = new CharacterComponent(0,0,lowerLegTex,_world, b2_dynamicBody, false);
	upperRightLeg = new CharacterComponent(0,0,upperLegTex,_world, b2_dynamicBody, false);
	lowerRightLeg = new CharacterComponent(0,0,lowerLegTex,_world, b2_dynamicBody, false);

	upperLeftLeg->transform->scale(-1, 1, 1);
	lowerLeftLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);
 
	     
	/*leftUpperArm  ->mesh->pushTexture2D(upperArmTex);
	leftLowerArm  ->mesh->pushTexture2D(lowerArmTex);
	leftHand	  ->mesh->pushTexture2D(handTex);
	rightUpperArm ->mesh->pushTexture2D(upperArmTex);
	rightLowerArm ->mesh->pushTexture2D(lowerArmTex);
	rightHand	  ->mesh->pushTexture2D(upperArmTex);
	upperLeftLeg  ->mesh->pushTexture2D(upperLegTex);
	lowerLeftLeg  ->mesh->pushTexture2D(lowerLegTex);
	upperRightLeg ->mesh->pushTexture2D(upperLegTex);
	lowerRightLeg ->mesh->pushTexture2D(lowerLegTex);*/
	

    attachJoints();
}

TestCharacter::~TestCharacter(){
}