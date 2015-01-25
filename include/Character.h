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
	Box2DWorld * world;

	CharacterComponent * torso;

	CharacterComponent * head;
	CharacterComponent * leftUpperArm;
	CharacterComponent * leftLowerArm;
	CharacterComponent * leftHand;
	CharacterComponent * rightUpperArm;
	CharacterComponent * rightLowerArm;
	CharacterComponent * rightHand;
	CharacterComponent * upperLeftLeg;
	CharacterComponent * lowerLeftLeg;
	CharacterComponent * upperRightLeg;
	CharacterComponent * lowerRightLeg;

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