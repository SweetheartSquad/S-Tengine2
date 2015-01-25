#pragma once

#include "MeshEntity.h"
#include <Box2D/Box2D.h>

class BitmapFont;
class Scene;
class Shader;
class Box2DWorld;
class Box2DSprite;
class CharacterComponent;
class Texture;

class ComponentTexture{
public:
	Texture * texture;
	float width;
	float height;

	ComponentTexture(Texture * _texture, float _width, float _height);
};

class Character : public MeshEntity{
public:

	static void init();
	static enum texture_packs{
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

	bool ai;
	bool reactiveFeet;
	static int16 gGroupIndex;
	int16 groupIndex;
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

	BitmapFont * text;

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

	std::vector<CharacterComponent *> components;

	explicit Character(Box2DWorld * _world, bool _ai = true);
	~Character();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void attachJoints();
	void unload() override;
	void load() override;
	void setShader(Shader * _shader);

	void addToScene(Scene * _scene);

	void translateComponents(glm::vec3 _translateVector);
};