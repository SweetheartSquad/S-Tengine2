#pragma once

#include <NodeBulletBody.h>
#include <MeshEntity.h>

class BulletWorld;

class BulletMeshEntity : public MeshEntity, public NodeBulletBody{
public:
	BulletMeshEntity(BulletWorld * _world, MeshInterface * _mesh);
	~BulletMeshEntity();

	void update(Step* _step) override;
};