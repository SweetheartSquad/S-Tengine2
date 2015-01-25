#pragma once

#include "MeshEntity.h"

class Shader;
class Box2DWorld;
class Box2DSprite;

class Character : public MeshEntity{
public:

	Box2DWorld * world;

	Box2DSprite * torso;

	Box2DSprite * head;
	Box2DSprite * leftUpperArm;
	Box2DSprite * leftLowerArm;
	Box2DSprite * leftHand;
	Box2DSprite * rightUpperArm;
	Box2DSprite * rightLowerArm;
	Box2DSprite * rightHand;
	Box2DSprite * upperLeftLeg;
	Box2DSprite * lowerLeftLeg;
	Box2DSprite * upperRightLeg;
	Box2DSprite * lowerRightLeg;

	explicit Character(Box2DWorld * _world);
	~Character();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;
	void attachJoints();
	void unload() override;
	void load() override;
	void setShader(Shader * _shader);
};