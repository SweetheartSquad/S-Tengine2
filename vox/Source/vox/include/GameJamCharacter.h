#pragma once

#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>

class GameJamCharacter : public Box2DSuperSprite{
public:
	bool ai;
	static void init();
	static enum texture_packs{
		kPUPPET,
		kKNIGHT,
		kMICHAEL,
		kMOUSTACHE,
		kAFRO,
		kIAN,
		kPONYTAIL
	};

	static std::vector<TextureSampler *> torsoTexPacks;
	static std::vector<TextureSampler *> headTexPacks;
	static std::vector<TextureSampler *> upperArmTexPacks;
	static std::vector<TextureSampler *> lowerArmTexPacks;
	static std::vector<TextureSampler *> handTexPacks;
	static std::vector<TextureSampler *> upperLegTexPacks;
	static std::vector<TextureSampler *> lowerLegTexPacks;

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

	Box2DSprite * torso;
	Box2DSprite * head;
	Box2DSprite * leftUpperArm;
	Box2DSprite * leftLowerArm;
	Box2DSprite * leftHand;
	Box2DSprite * rightUpperArm;
	Box2DSprite * rightLowerArm;
	Box2DSprite * rightHand;
	Box2DSprite * leftUpperLeg;
	Box2DSprite * leftLowerLeg;
	Box2DSprite * rightUpperLeg;
	Box2DSprite * rightLowerLeg;

	explicit GameJamCharacter(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupBits = 0, bool _ai = true);
	
	~GameJamCharacter();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void attachJoints();
	void setShader(Shader* _shader, bool _configureDefaultVertexAttributes) override;
	void addToLayeredScene(LayeredScene* _scene, unsigned long int _layer) override;
};