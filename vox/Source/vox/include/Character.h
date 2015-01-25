#pragma once

#include "MeshEntity.h"

class Scene;
class Shader;
class Box2DWorld;
class Box2DSprite;
class CharacterComponent;

class Character : public MeshEntity{
public:
	float componentScale;
	
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

	float ratioX_wrist_to_elbow;
	float ratioY_wrist_to_elbow;
	float ratioX_elbow_to_wrist;
	float ratioY_elbow_to_wrist;
	
	float ratioX_hip_to_torso;
	float ratioY_hip_to_torso;
	float ratioX_torso_to_hip;
	float ratioY_torso_to_hip;
	
	float ratioX_knee_to_hip;
	float ratioY_knee_to_hip;
	float ratioX_hip_to_knee;
	float ratioY_hip_to_knee;

	float hipWidth;


	Box2DWorld * world;

	CharacterComponent * torso;

	CharacterComponent * head;
	CharacterComponent * leftUpperArm;
	CharacterComponent * leftLowerArm;
	CharacterComponent * leftHand;
	CharacterComponent * rightUpperArm;
	CharacterComponent * rightLowerArm;
	CharacterComponent * rightHand;
	CharacterComponent * leftUpperLeg;
	CharacterComponent * leftLowerLeg;
	CharacterComponent * rightUpperLeg;
	CharacterComponent * rightLowerLeg;

	explicit Character(Box2DWorld * _world);
	~Character();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void attachJoints();
	void unload() override;
	void load() override;
	void setShader(Shader * _shader);

	void addToScene(Scene * _scene);
};