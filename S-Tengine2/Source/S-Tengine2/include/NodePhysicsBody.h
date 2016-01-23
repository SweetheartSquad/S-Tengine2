#pragma once

#include <Entity.h>

// NOTE: physics bodies override Entity's update and don't call it
// This is because the MeshEntity varieties will also call it,
// causing problems in the most common use case. If you want the
// physics body to update its children, you have to extend this class
// and call it yourself in an overriden update call
class NodePhysicsBody abstract : public virtual Entity{
protected:

	// whether the physics body has been directly adjusted since the last update
	// this is used to help determine whether the childTransform needs to be updated
	// to match the physics body's position
	bool directAdjustment;

public:
	NodePhysicsBody();

	virtual void update(Step * _step);

	// if _relative is true, translates the physics body BY _translation
	// if _relative is false, translates the physics body TO _translation
	// sets the directAdjustment flag to true, telling us that the childTransform needs to be updated
	virtual void translatePhysical(glm::vec3 _translation, bool _relative = true);

	// Re-aligns the childTransform with the physics body's transformation properties
	// this should be called when the body is active or there has been a direct adjustment
	// Clears the directAdjustment flag
	virtual void realign();
};