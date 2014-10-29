#pragma once 

#include "MeshInterface.h"

class VoxelMesh : public MeshInterface{

public:

	float resolution;

	explicit VoxelMesh(GLenum _drawMode);
	~VoxelMesh();

	void configureExtras(Shader* _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights) override;
};