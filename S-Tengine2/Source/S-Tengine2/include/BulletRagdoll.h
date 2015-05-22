#pragma once

#include <Entity.h>

class BulletMeshEntity;
class BulletWorld;
class Shader;

class BulletRagdoll : public Entity{
public:
	BulletMeshEntity * upperbody;
	BulletMeshEntity * lowerbody;
	BulletMeshEntity * head;
	BulletMeshEntity * upperlegLeft;
	BulletMeshEntity * upperlegRight;
	BulletMeshEntity * lowerlegLeft;
	BulletMeshEntity * lowerlegRight;
	BulletMeshEntity * upperarmLeft;
	BulletMeshEntity * upperarmRight;
	BulletMeshEntity * lowerarmLeft;
	BulletMeshEntity * lowerarmRight;


	BulletRagdoll(BulletWorld * _world);

	void setShader(Shader * _shader, bool _default);
};