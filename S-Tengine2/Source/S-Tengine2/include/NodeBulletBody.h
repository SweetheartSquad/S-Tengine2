#pragma once

#include <NodePhysicsBody.h>
#include <BulletWorld.h>

class TriMesh;
class Texture;

// NOTE: physics bodies override Entity's update and don't call it
// This is because the MeshEntity varieties will also call it,
// causing problems in the most common use case. If you want the
// physics body to update its children, you have to extend this class
// and call it yourself in an overriden update call
class NodeBulletBody : public virtual NodePhysicsBody{
protected:
	btVector3 internalPos;

public:
	BulletWorld * world;
	btRigidBody * body;
	btCollisionShape * shape;
	//Max velocity. Should always be set as positive. -1 means no limit
	btVector3 maxVelocity;

	NodeBulletBody(BulletWorld * _world);
	virtual ~NodeBulletBody();

	void setColliderAsStaticPlane(float _normalX = 0.f, float _normalY = 1.f, float _normalZ = 0.f, float _distanceFromOrigin = 0.f);
	void setColliderAsBox(float _halfX = 0.5f, float _halfY = 0.5f, float _halfZ = 0.5f);
	void setColliderAsSphere(float _radius = 0.5f);
	void setColliderAsCapsule(float _radius = 0.5f, float _height = 0.5f);
	void setColliderAsMesh(TriMesh * _colliderMesh, bool _convex);
	// upAxis = 0, 1, or 2 = x, y, or z axis
	void setColliderAsHeightMap(Texture * _heightMap, glm::vec3 _scale, unsigned long int _upAxis = 1);
	// a mass of zero makes it static
	virtual void createRigidBody(float _mass, unsigned short int _collisionGroup = -1, unsigned short int _collisionMask = -1);

	// directly adjusts the rigid body's transform and then updates the childTransform to match
	// a flag is set for the childTransform to be updated later
	// note: rotates _angle degrees around the axis (_x, _y, _z)
	virtual void rotatePhysical(float _angle, float _x, float _y, float _z, bool _relative = true);

	virtual void update(Step * _step) override;
	
	virtual void applyForce(glm::vec3 _force, glm::vec3 _point) override;
	virtual void applyLinearImpulse(glm::vec3 _impulse, glm::vec3 _point) override;
	
	virtual void applyForceToCenter(glm::vec3 _force) override;
	virtual void applyLinearImpulseToCenter(glm::vec3 _impulse) override;

	virtual glm::vec3 getPhysicsBodyCenter() override;
	
	virtual void translatePhysical(glm::vec3 _translation, bool _relative = true) override;
	virtual void realign() override;
};