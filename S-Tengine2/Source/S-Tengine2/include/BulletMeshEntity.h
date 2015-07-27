#pragma once

#include <NodeBulletBody.h>
#include <MeshEntity.h>

class BulletWorld;

class BulletMeshEntity : public NodeBulletBody, public MeshEntity{
public:
	BulletMeshEntity(BulletWorld * _world, MeshInterface * _mesh);

	void update(Step* _step) override;

	void setColliderAsBoundingBox();

	// if _encompassBox is true, the sphere is the smallest one that completely encompasses the bounding box
	// if _encompassBox is false, the sphere is the largest one that can be completely encompassed by the bounding box
	void setColliderAsBoundingSphere(bool _encompassBox = false);
};