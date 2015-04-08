#pragma once

#include <Box2D/Box2D.h>

#include "Box2DSuperSprite.h"
#include "PuppetGame.h"
#include <vector>

class PuppetTexturePack;
class Box2DSprite;
class Box2DWorld;
class Item;
class BehaviourManager;

class PuppetCharacter : public Box2DSuperSprite {
public:
	bool ai;
	bool canJump;
	bool dead;
	bool deathPending;
	bool justTookDamage;

	double actionThrottle;
	double lastActionTime;

	signed long int id;

	float targetRoll;
	float score;
	float lastUpdateScore;

	float health;

	float control;
	float ghostPosition;

	PuppetTexturePack * texPack;

	Box2DSprite * head;
	Box2DSprite * torso;
	Box2DSprite * armLeft;
	Box2DSprite * armRight;
	Box2DSprite * handLeft;
	Box2DSprite * handRight;

	Box2DSprite * face;
	Box2DSprite * headgear;
	Box2DSprite * itemHolder;
	Box2DSprite * indicator;

	std::vector<PuppetGame::BOX2D_CATEGORY> collisionTypes;

	static bool compareByScore(PuppetCharacter * _a, PuppetCharacter * _b);
	
	void init();
	PuppetCharacter(PuppetTexturePack * _texturePack, float _ghostPosition, bool _ai, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~PuppetCharacter();
	virtual PuppetCharacter * clone(Box2DWorld * _world);

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	void update(Step* _step) override;
	void unload() override;
	void load() override;

	void delegateToContactComplete(std::function<void(PuppetCharacter *)> _func);

	virtual void jump();
	//Called when the controller is thrust forward
	virtual void action(bool _forceDrop = false);
	virtual void die();

	void removeCollision(PuppetGame::BOX2D_CATEGORY _category);
	void addCollision(PuppetGame::BOX2D_CATEGORY _category);
	bool isCollidingWith(PuppetGame::BOX2D_CATEGORY _category);


    virtual void takeDamage(float _damage);

	Item * itemToPickup;
	Item * heldItem;
	b2WeldJoint * itemJoint;
	virtual void pickupItem(Item * _item);

	BehaviourManager * behaviourManager;

	virtual void setShader(Shader * _shader, bool _configureDefaultAttributes) override;

	virtual void translateComponents(glm::vec3 _translationVector) override;

private:
	std::function<void(PuppetCharacter *)> contactDelegate;
};
