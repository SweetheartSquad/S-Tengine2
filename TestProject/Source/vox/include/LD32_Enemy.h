#pragma once

#include <Box2DMeshEntity.h>

class LD32_Enemy : public Box2DMeshEntity {
public:
	bool heDed;
	LD32_Enemy(Box2DWorld * _world);
};