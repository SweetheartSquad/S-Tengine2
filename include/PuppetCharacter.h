#pragma once

#include <Box2D/Box2D.h>

#include "Character.h"

class CharacterComponent;
class Box2DWorld;

class PuppetCharacter : public Character{
public:

	float targetRoll;
	
	CharacterComponent * torso;
	CharacterComponent * head;
	CharacterComponent * leftUpperArm;
	CharacterComponent * leftHand;
	CharacterComponent * rightUpperArm;
	CharacterComponent * rightHand;

	float ratioX_neck_to_torso;
	float ratioY_neck_to_torso;

	float ratioX_torso_to_neck;
	float ratioY_torso_to_neck;

	float ratioX_shoulder_to_torso;
	float ratioY_shoulder_to_torso;
	float ratioX_torso_to_shoulder;
	float ratioY_torso_to_shoulder;
	
	float ratioX_elbow_to_shoulder;
	float ratioY_elbow_to_shoulder;
	float ratioX_shoulder_to_elbow;
	float ratioY_shoulder_to_elbow;

	PuppetCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, bool _ai = true);
	~PuppetCharacter();	
	
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

};