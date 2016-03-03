#pragma once

#include <Entity.h>
#include <MeshEntity.h>

class ComponentShaderBase;
class AnimationJoint : public Entity{
private:
	ComponentShaderBase * shaderVis;
public:
	// position (relative to parent joint)
	glm::vec2 pos; 
	
	bool limitsEnabled;

	// upper limit on local angle (upper limit takes precedence)
	float angleLimitLower;
	// upper limit on local angle (takes precedence over lower limit)
	float angleLimitUpper;
	// list of joints which branch off of this one (i.e. bones which have this joint as a root)
	std::vector<AnimationJoint *> outJoints;

	explicit AnimationJoint(glm::vec2 _pos);
	virtual ~AnimationJoint();

	// finds the corresponding angle within the range [-180, 180] and clamps it to within the joint's specified limits (if enabled)
	void setAngle(float _angle);
	float getAngle();

	// enabled angle limits and sets them to the provided values
	void setAngleLimits(float _lower, float _upper);
	
	// adds _j as a child of this joint
	// adds _j to the out-list of this joint
	// (this will create a new bone which branches off of this joint)
	void addJoint(AnimationJoint * _j, bool _behind = false);

	void setJointsVisible(bool _visible);

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
protected:
	float angle; // angle in parent space

	MeshEntity * lineVis;
	MeshEntity * pointVis;
};

// this class doesn't actually do anything yet; just extends Entity
class AnimationRig : public Entity{};