#pragma once

#include <MeshEntity.h>

class Shader;

class Cloud : public MeshEntity{
public:
	Cloud(Shader * _shader);

	void update(Step * _step) override;
};