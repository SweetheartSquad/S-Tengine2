#pragma once

#include "Character.h"
#include <Box2D/Box2D.h>

class GameJamCharacter : public Character{
public:

	static void init();
	static enum texture_packs{
		kPUPPET,
		kMICHAEL,
		kMOUSTACHE,
		kAFRO,
		kIAN,
		kPONYTAIL
	};

	static std::vector<ComponentTexture *> torsoTexPacks;
	static std::vector<ComponentTexture *> headTexPacks;
	static std::vector<ComponentTexture *> upperArmTexPacks;
	static std::vector<ComponentTexture *> lowerArmTexPacks;
	static std::vector<ComponentTexture *> handTexPacks;
	static std::vector<ComponentTexture *> upperLegTexPacks;
	static std::vector<ComponentTexture *> lowerLegTexPacks;

	bool reactiveFeet;
	bool reactiveBody;
	
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

	BitmapFont * text;

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

	explicit GameJamCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, bool _ai = true);
	
	~GameJamCharacter();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void attachJoints();
	void setShader(Shader* _shader, bool _configureDefaultVertexAttributes) override;
	void addToScene(Scene* _scene) override;
};