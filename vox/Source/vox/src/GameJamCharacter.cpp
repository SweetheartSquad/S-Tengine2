#pragma once

#include "GameJamCharacter.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "Box2DSprite.h"
#include "Scene.h"
#include "LayeredScene.h"
#include "BitmapFont.h"

#include "Texture.h"
#include <PuppetResourceManager.h>
#include <TextureSampler.h>

std::vector<TextureSampler *> GameJamCharacter::torsoTexPacks;
std::vector<TextureSampler *> GameJamCharacter::headTexPacks;
std::vector<TextureSampler *> GameJamCharacter::upperArmTexPacks;
std::vector<TextureSampler *> GameJamCharacter::lowerArmTexPacks;
std::vector<TextureSampler *> GameJamCharacter::handTexPacks;
std::vector<TextureSampler *> GameJamCharacter::upperLegTexPacks;
std::vector<TextureSampler *> GameJamCharacter::lowerLegTexPacks;

void GameJamCharacter::init(){
	//torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/pTorso.png", 512, 512, true, true),	123,	185)); 
//	torsoTexPacks.push_back(PuppetResourceManager::goldenBreastPlateStick); 
	torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelTorso.png", 512, 512, true, true),	145,	362)); 
	torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheTorso.png", 512, 512, true, true),	160,	290));
	torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroTorso.png", 512, 512, true, true),		190,	325));
	torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieTorso.png", 512, 512, true, true),	175,	285));
	torsoTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailTorso.png", 512, 512, true, true),	160,	355));
	
	//headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/pHead.png", 512, 512, true, true),   214,	186));
	//headTexPacks.push_back(PuppetResourceManager::head1); 
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelHead.png", 512, 512, true, true),   200,	270));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheHead.png", 512, 512, true, true), 160,	270));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroHead.png", 512, 512, true, true),	   260,	250));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieHead.png", 512, 512, true, true),	   170,	300));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailHead.png", 512, 512, true, true),  155,	365));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MonsterHead.png", 512, 512, true, true),	222,	393));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/BeardHead.png", 512, 512, true, true),		165,	342));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/FringeHead.png", 512, 512, true, true),		228,	257));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/BlankHead.png", 512, 512, true, true),		169,	247));
	headTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/BowlcutHead.png", 512, 512, true, true),		272,	243));

	//upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/pArm.png", 512, 512, true, true),	45,	155));
	//upperArmTexPacks.push_back(PuppetResourceManager::goldenArm); 
	upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelUpperArm.png", 512, 512, true, true),	55,	205));
	upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheUA.png", 512, 512, true, true),	55,	205));
	upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroUA.png", 512, 512, true, true),			50,	200));
	upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieUA.png", 512, 512, true, true),		65,	165));
	upperArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailUA.png", 512, 512, true, true),		60,	200));
	
	//lowerArmTexPacks.push_back(nullptr);
	//lowerArmTexPacks.push_back(nullptr);
	lowerArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelLowerArm.png", 512, 512, true, true), 39,	144));
	lowerArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheLA.png", 512, 512, true, true),	40,	145));
	lowerArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroLA.png", 512, 512, true, true),			40,	145));
	lowerArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieLA.png", 512, 512, true, true),		55,	165));
	lowerArmTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailLA.png", 512, 512, true, true),		55,	160));
	
	//handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/pHand.png", 512, 512, true, true), 80,	85));
	//handTexPacks.push_back(PuppetResourceManager::hand1);
	handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelHand.png", 512, 512, true, true),	50,	82));
	handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheHand.png", 512, 512, true, true),	55,	85));
	handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroHand.png", 512, 512, true, true),		55,	70));
	handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieHand.png", 512, 512, true, true),		55,	85));
	handTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailHand.png", 512, 512, true, true),	60,	80));
	
	//upperLegTexPacks.push_back(nullptr);
	//upperLegTexPacks.push_back(nullptr);
	upperLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelUpperLeg.png", 512, 512, true, true), 64,	218));
	upperLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheUL.png", 512, 512, true, true),	 70,	215));
	upperLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroUL.png", 512, 512, true, true),			 60,	200));
	upperLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieUL.png", 512, 512, true, true),		 50,	205));
	upperLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailUL.png", 512, 512, true, true),		 80,	185));
	
	//lowerLegTexPacks.push_back(nullptr);
	//lowerLegTexPacks.push_back(nullptr);
	lowerLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MichaelLowerLeg.png", 512, 512, true, true), 135,	281));
	lowerLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/MoustacheLL.png", 512, 512, true, true),	 135,	285));
	lowerLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/AfroLL.png", 512, 512, true, true),			 105,	255));
	lowerLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/HoodieLL.png", 512, 512, true, true),		 115,	240));
	lowerLegTexPacks.push_back(new TextureSampler(new Texture("../assets/character components/PonytailLL.png", 512, 512, true, true),		 110,	205));
}

GameJamCharacter::GameJamCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, bool _ai) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	text(new BitmapFont(new Texture("../assets/arial.bmp", 1024, 1024, true, true), 32, 16, 16 )),
	reactiveFeet(true),
	reactiveBody(true),
	head(nullptr),
	leftUpperArm(nullptr),
	leftLowerArm(nullptr),
	leftHand(nullptr),
	rightUpperArm(nullptr),
	rightLowerArm(nullptr),
	rightHand(nullptr),
	leftUpperLeg(nullptr),
	leftLowerLeg(nullptr),
	rightUpperLeg(nullptr),
	rightLowerLeg(nullptr),
	ai(ai)
{
	ratioX_neck_to_torso = 0.0f;
	ratioY_neck_to_torso = 0.8f;
	ratioX_torso_to_neck = 0.0f;
	ratioY_torso_to_neck = 0.8f;
	
	ratioX_shoulder_to_torso = 0.f;
	ratioY_shoulder_to_torso = 1.f;
	ratioX_torso_to_shoulder = 0.8f;
	ratioY_torso_to_shoulder = 0.5f;
	
	ratioX_elbow_to_shoulder = 0.f;
	ratioY_elbow_to_shoulder = 0.7f;
	ratioX_shoulder_to_elbow = 0.f;
	ratioY_shoulder_to_elbow = 0.7f;
	
	ratioX_wrist_to_elbow = 0.f;
	ratioY_wrist_to_elbow = 0.8f;
	ratioX_elbow_to_wrist = 0.f;
	ratioY_elbow_to_wrist = 0.8f;
	
	ratioX_hip_to_torso = 0.f;
	ratioY_hip_to_torso = 1.f;
	ratioX_torso_to_hip = 0.55f;
	ratioY_torso_to_hip = 0.7f;
	
	ratioX_knee_to_hip = 1.0f;
	ratioY_knee_to_hip = 0.8f;
	ratioX_hip_to_knee = 1.0f;
	ratioY_hip_to_knee = 0.8f;

	text->transform->translate(0, 0, 6);
	text->kerning = 0.75f;
	text->setSizeMod(0.85f);

	components.push_back(&leftUpperArm);
	components.push_back(&leftLowerArm);
	components.push_back(&leftHand);
	components.push_back(&leftUpperLeg);
	components.push_back(&leftLowerLeg);
	components.push_back(&torso);
	components.push_back(&head);
	components.push_back(&rightUpperLeg);
	components.push_back(&rightLowerLeg);
	components.push_back(&rightUpperArm);
	components.push_back(&rightLowerArm);
	components.push_back(&rightHand);
	rootComponent = torso;

}

GameJamCharacter::~GameJamCharacter(){
}

void GameJamCharacter::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Box2DSuperSprite::render(_matrixStack, _renderOptions);
}

void GameJamCharacter::update(Step * _step){
	Box2DSuperSprite::update(_step);
	if(reactiveBody){
		//neck
		b2RevoluteJoint * neck = ((b2RevoluteJoint *)head->body->GetJointList()->joint);
		float angle = neck->GetJointAngle();

		neck->SetMotorSpeed(-angle*360);
		neck->SetMaxMotorTorque(head->body->GetMass()*750*(std::abs(angle)*5));

		//body
		float bodAngle = torso->body->GetAngle();
		torso->body->SetAngularVelocity(-bodAngle*10);
		if(torso->body->GetPosition().y < 5){
			torso->applyLinearImpulseUp(250);
		}
	}
	
	/*if(angle < neck->GetLowerLimit()/2 || angle > neck->GetUpperLimit()/2){
		neck->SetMotorSpeed(angle < 0 ? 0.1 : -0.1);
		neck->SetMaxMotorTorque(head->body->GetMass()*1000);
	}else{
		neck->SetMotorSpeed(0);
		neck->SetMaxMotorTorque(0);
	}*/

	if(ai && reactiveFeet){
		b2ContactEdge * stuff = leftLowerLeg->body->GetContactList();
		if(stuff != nullptr && stuff->contact->IsTouching()){
			int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
			int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

			if(type1 != type2){
				if(leftLowerLeg->body->GetLinearVelocity().y <= 0.1){
					leftLowerLeg->applyLinearImpulseUp(125);	
					torso->applyLinearImpulseUp(125);	
				}
			}
		}
		stuff = rightLowerLeg->body->GetContactList();
		if(stuff != nullptr && stuff->contact->IsTouching()){
			int type1 = (int)stuff->contact->GetFixtureA()->GetUserData();
			int type2 = (int)stuff->contact->GetFixtureB()->GetUserData();

			if(type1 != type2){
				if(rightLowerLeg->body->GetLinearVelocity().y <= 0.1){
					rightLowerLeg->applyLinearImpulseUp(125);
					torso->applyLinearImpulseUp(125);	
				}
			}
		}
	}
	text->transform->translate((torso->transform->getTranslationVector().x) - 2.0f, 7, text->transform->getTranslationVector().z, false);
}

void GameJamCharacter::attachJoints(){

	/*for(NodeChild * s : children){
		dynamic_cast<Box2DSprite *>(s)->transform->scale(5,5,1);
	}*/

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	torso->createFixture(sf);
	head->createFixture(sf);
	/*

	// sensor
	b2PolygonShape tShape;
	tShape.SetAsBox(torso->width*std::abs(transform->scaleVector.x)*torso->scale, std::abs(torso->height*transform->scaleVector.y)*torso->scale);

	b2Fixture * s = torso->body->CreateFixture(&tShape, 1); // physical fixture
	s->SetSensor(true);
	s->SetFilterData(sf);*/

	
	
	float correctedTorsoWidth = torso->getCorrectedWidth();
	float correctedTorsoHeight = torso->getCorrectedHeight();

	// neck
	b2RevoluteJointDef jth;
	jth.bodyA = torso->body;
	jth.bodyB = head->body;
	jth.localAnchorA.Set(ratioX_torso_to_neck * correctedTorsoWidth, ratioY_torso_to_neck * correctedTorsoHeight);
	jth.localAnchorB.Set(ratioX_neck_to_torso * head->getCorrectedWidth(), ratioY_neck_to_torso * -head->getCorrectedHeight());
	jth.collideConnected = false;
	jth.enableLimit = true;
	jth.enableMotor = true;
	jth.maxMotorTorque = 0;
	jth.motorSpeed = 0;
	jth.referenceAngle = 0;
	jth.lowerAngle = -glm::radians(45.f);
	jth.upperAngle = glm::radians(45.f);
	world->b2world->CreateJoint(&jth);

	// left shoulder
	leftUpperArm->createFixture(sf);
	b2RevoluteJointDef latj;
	latj.bodyA = torso->body;
	latj.bodyB = leftUpperArm->body;
	latj.localAnchorA.Set(-ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	latj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	latj.collideConnected = false;
	latj.enableLimit = true;
	latj.referenceAngle = glm::radians(0.f);
	latj.lowerAngle = glm::radians(-120.f);
	latj.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&latj);
	

	// right shoulder
	rightUpperArm->createFixture(sf);
	b2RevoluteJointDef ratj;
	ratj.bodyA = torso->body;
	ratj.bodyB = rightUpperArm->body;
	ratj.localAnchorA.Set(ratioX_torso_to_shoulder * correctedTorsoWidth, ratioY_torso_to_shoulder * correctedTorsoHeight);
	ratj.localAnchorB.Set(ratioX_shoulder_to_torso * leftUpperArm->getCorrectedWidth(), ratioY_shoulder_to_torso * leftUpperArm->getCorrectedHeight());
	ratj.collideConnected = false;
	ratj.enableLimit = true;
	ratj.referenceAngle = glm::radians(0.f);
	ratj.lowerAngle = glm::radians(-10.f);
	ratj.upperAngle = glm::radians(120.f);
	world->b2world->CreateJoint(&ratj);
	
	
	// left elbow
	leftLowerArm->createFixture(sf);
	b2RevoluteJointDef lelsj;
	lelsj.bodyA = leftUpperArm->body;
	lelsj.bodyB = leftLowerArm->body;
	lelsj.localAnchorA.Set(-ratioX_shoulder_to_elbow * leftUpperArm->getCorrectedWidth(), -ratioY_shoulder_to_elbow * leftUpperArm->getCorrectedHeight());
	lelsj.localAnchorB.Set(-ratioX_elbow_to_shoulder * leftLowerArm->getCorrectedWidth(), ratioY_elbow_to_shoulder * leftLowerArm->getCorrectedHeight());
	lelsj.collideConnected = false;
	lelsj.enableLimit = true;
	lelsj.referenceAngle = glm::radians(0.f);
	lelsj.lowerAngle = glm::radians(-10.f);
	lelsj.upperAngle = glm::radians(90.f);
	world->b2world->CreateJoint(&lelsj);

	// right elbow
	rightLowerArm->createFixture(sf);
	b2RevoluteJointDef rersj;
	rersj.bodyA = rightUpperArm->body;
	rersj.bodyB = rightLowerArm->body;
	rersj.localAnchorA.Set(-ratioX_shoulder_to_elbow * rightUpperArm->getCorrectedWidth(), -ratioY_shoulder_to_elbow * rightUpperArm->getCorrectedHeight());
	rersj.localAnchorB.Set(ratioX_elbow_to_shoulder * rightLowerArm->getCorrectedWidth(), ratioY_elbow_to_shoulder * rightLowerArm->getCorrectedHeight());
	rersj.collideConnected = false;
	rersj.enableLimit = true;
	rersj.referenceAngle = glm::radians(0.f);
	rersj.lowerAngle = glm::radians(-90.f);
	rersj.upperAngle = glm::radians(10.f);
	world->b2world->CreateJoint(&rersj);

	// left hand
	leftHand->createFixture(sf);
	b2RevoluteJointDef lhlej;
	lhlej.bodyA = leftLowerArm->body;
	lhlej.bodyB = leftHand->body;
	lhlej.localAnchorA.Set(-ratioX_elbow_to_wrist * leftLowerArm->getCorrectedWidth(), -ratioY_elbow_to_wrist * leftLowerArm->getCorrectedHeight());
	lhlej.localAnchorB.Set(-ratioX_wrist_to_elbow * leftHand->getCorrectedWidth(), ratioY_wrist_to_elbow * leftHand->getCorrectedHeight());
	lhlej.collideConnected = false;
	lhlej.enableLimit = true;
	lhlej.referenceAngle = glm::radians(0.f);
	lhlej.lowerAngle = glm::radians(-20.f);
	lhlej.upperAngle = glm::radians(20.f);
	world->b2world->CreateJoint(&lhlej);

	// right hand
	rightHand->createFixture(sf);
	b2RevoluteJointDef rhrej;
	rhrej.bodyA = rightLowerArm->body;
	rhrej.bodyB = rightHand->body;
	rhrej.localAnchorA.Set(-ratioX_elbow_to_wrist * rightLowerArm->getCorrectedWidth(), -ratioY_elbow_to_wrist * rightLowerArm->getCorrectedHeight());
	rhrej.localAnchorB.Set(ratioX_wrist_to_elbow * rightHand->getCorrectedWidth(), ratioY_wrist_to_elbow * rightHand->getCorrectedHeight());
	rhrej.collideConnected = false;
	rhrej.enableLimit = true;
	rhrej.referenceAngle = glm::radians(0.f);
	rhrej.lowerAngle = glm::radians(-20.f);
	rhrej.upperAngle = glm::radians(20.f);
	world->b2world->CreateJoint(&rhrej);

	// left hip
	leftUpperLeg->createFixture(sf);
	b2RevoluteJointDef lltj;
	lltj.bodyA = torso->body;
	lltj.bodyB = leftUpperLeg->body;
	lltj.localAnchorA.Set(-ratioX_torso_to_hip * correctedTorsoWidth, -ratioY_torso_to_hip * correctedTorsoHeight);
	lltj.localAnchorB.Set(ratioX_hip_to_torso * leftUpperLeg->getCorrectedWidth(),  ratioY_hip_to_torso * leftUpperLeg->getCorrectedHeight());
	lltj.collideConnected = false;
	lltj.enableLimit = true;
	lltj.lowerAngle = glm::radians(-100.f);
	lltj.upperAngle = glm::radians(25.f);
	world->b2world->CreateJoint(&lltj);
	
	// right hip
	rightUpperLeg->createFixture(sf);
	b2RevoluteJointDef lrtj;
	lrtj.bodyA = torso->body;
	lrtj.bodyB = rightUpperLeg->body;
	lrtj.localAnchorA.Set(ratioX_torso_to_hip * correctedTorsoWidth, -ratioY_torso_to_hip * correctedTorsoHeight);
	lrtj.localAnchorB.Set(ratioX_hip_to_torso * leftUpperLeg->getCorrectedWidth(), ratioY_hip_to_torso * rightUpperLeg->getCorrectedHeight());
	lrtj.collideConnected = false;
	lrtj.enableLimit = true;
	lrtj.lowerAngle = glm::radians(-25.f);
	lrtj.upperAngle = glm::radians(100.f);
	world->b2world->CreateJoint(&lrtj);

	// left knee
	leftLowerLeg->createFixture(sf);
	b2RevoluteJointDef llltj;
	llltj.bodyA = leftUpperLeg->body;
	llltj.bodyB = leftLowerLeg->body;
	llltj.localAnchorA.Set(ratioX_hip_to_knee * leftUpperLeg->getCorrectedWidth(), -ratioY_hip_to_knee * leftUpperLeg->getCorrectedHeight());
	llltj.localAnchorB.Set(ratioX_knee_to_hip * leftLowerLeg->getCorrectedWidth(), ratioY_knee_to_hip * leftLowerLeg->getCorrectedHeight());
	llltj.collideConnected = false;
	llltj.enableLimit = true;
	llltj.lowerAngle = glm::radians(0.f);
	llltj.upperAngle = glm::radians(75.f);
	world->b2world->CreateJoint(&llltj);
	
	// right knee
	rightLowerLeg->createFixture(sf);
	b2RevoluteJointDef lrltj;
	lrltj.bodyA = rightUpperLeg->body;
	lrltj.bodyB = rightLowerLeg->body;
	lrltj.localAnchorA.Set(-ratioX_hip_to_knee * rightUpperLeg->getCorrectedWidth(), -ratioY_hip_to_knee * rightUpperLeg->getCorrectedHeight());
	lrltj.localAnchorB.Set(-ratioX_knee_to_hip * rightLowerLeg->getCorrectedWidth(), ratioY_knee_to_hip * rightLowerLeg->getCorrectedHeight());
	lrltj.collideConnected = false;
	lrltj.enableLimit = true;
	lrltj.lowerAngle = glm::radians(-75.f);
	lrltj.upperAngle = glm::radians(0.f);
	world->b2world->CreateJoint(&lrltj);

	
	float t = groupIndex*0.000001f;

	torso		   ->setTranslationPhysical(0.f, 0.f, 0);
	head		   ->setTranslationPhysical(0.f, 0.f, 0);
	leftUpperArm   ->setTranslationPhysical(0.f, 0.f, 0);
	leftLowerArm   ->setTranslationPhysical(0.f, 0.f, 0);
	leftHand	   ->setTranslationPhysical(0.f, 0.f, 0);
	rightUpperArm  ->setTranslationPhysical(0.f, 0.f, 0);
	rightLowerArm  ->setTranslationPhysical(0.f, 0.f, 0);
	rightHand	   ->setTranslationPhysical(0.f, 0.f, 0);
	leftUpperLeg   ->setTranslationPhysical(0.f, 0.f, 0);
	leftLowerLeg   ->setTranslationPhysical(0.f, 0.f, 0);
	rightUpperLeg  ->setTranslationPhysical(0.f, 0.f, 0);
	rightLowerLeg  ->setTranslationPhysical(0.f, 0.f, 0);
	text->transform->translate(0, 0, -2.3f + t);

	torso->body->SetLinearDamping(0.9f);
	torso->body->SetGravityScale(0.5f);

	setUserData(this);
}


void GameJamCharacter::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	Box2DSuperSprite::setShader(_shader, _configureDefaultVertexAttributes);
	text->setShader(_shader ,_configureDefaultVertexAttributes);
}

void GameJamCharacter::addToLayeredScene(LayeredScene * _scene, unsigned long int _layer){
	Box2DSuperSprite::addToLayeredScene(_scene, _layer);
	_scene->addChild(text, _layer);
}