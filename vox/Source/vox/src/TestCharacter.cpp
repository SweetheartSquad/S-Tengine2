#pragma once

#include "TestCharacter.h"
#include "Box2DSprite.h"
#include "Texture.h"

TestCharacter::TestCharacter(Box2DWorld * _world):
	Character(_world),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	torso	      = new Box2DSprite(_world, b2_dynamicBody, false);
	head		  = new Box2DSprite(_world, b2_dynamicBody, false);
	leftUpperArm  = new Box2DSprite(_world, b2_dynamicBody, false);
	leftLowerArm  = new Box2DSprite(_world, b2_dynamicBody, false);
	leftHand	  = new Box2DSprite(_world, b2_dynamicBody, false);
	rightUpperArm = new Box2DSprite(_world, b2_dynamicBody, false);
	rightLowerArm = new Box2DSprite(_world, b2_dynamicBody, false);
	rightHand	  = new Box2DSprite(_world, b2_dynamicBody, false);
	upperLeftLeg  = new Box2DSprite(_world, b2_dynamicBody, false);
	lowerLeftLeg  = new Box2DSprite(_world, b2_dynamicBody, false);
	upperRightLeg = new Box2DSprite(_world, b2_dynamicBody, false);
	lowerRightLeg = new Box2DSprite(_world, b2_dynamicBody, false);

	upperLeftLeg->transform->scale(-1, 1, 1);
	lowerLeftLeg->transform->scale(-1, 1, 1); 
	leftUpperArm->transform->scale(-1, 1, 1); 
	leftLowerArm->transform->scale(-1, 1, 1);  
	leftHand->transform->scale(-1, 1, 1);

	Texture * torsoTex = new Texture("../assets/MichaelTorso.png", 512, 512, true, true);     
	Texture * headTex = new Texture("../assets/MichaelHead.png", 512, 512, true, true);  		 
	Texture * upperArmTex = new Texture("../assets/MichaelUpperArm.png", 512, 512, true, true);  
	Texture * lowerArmTex = new Texture("../assets/MichaelLowerArm.png", 512, 512, true, true);  
	Texture * handTex = new Texture("../assets/MichaelHand.png", 512, 512, true, true);  	 
	Texture * upperLegTex = new Texture("../assets/MichaelUpperLeg.png", 512, 512, true, true);  
	Texture * lowerLegTex = new Texture("../assets/MichaelLowerLeg.png", 512, 512, true, true);  

	torso		  ->mesh->pushTexture2D(torsoTex);	     
	head		  ->mesh->pushTexture2D(headTex);
	leftUpperArm  ->mesh->pushTexture2D(upperArmTex);
	leftLowerArm  ->mesh->pushTexture2D(lowerArmTex);
	leftHand	  ->mesh->pushTexture2D(handTex);
	rightUpperArm ->mesh->pushTexture2D(upperArmTex);
	rightLowerArm ->mesh->pushTexture2D(lowerArmTex);
	rightHand	  ->mesh->pushTexture2D(upperArmTex);
	upperLeftLeg  ->mesh->pushTexture2D(upperLegTex);
	lowerLeftLeg  ->mesh->pushTexture2D(lowerLegTex);
	upperRightLeg ->mesh->pushTexture2D(upperLegTex);
	lowerRightLeg ->mesh->pushTexture2D(lowerLegTex);

    attachJoints();
}

TestCharacter::~TestCharacter(){
}