#pragma once 

#include "MeshInterface.h"

class VoxelMesh : public MeshInterface{

public:

	float resolution;

	explicit VoxelMesh(GLenum _drawMode);
	explicit VoxelMesh(MeshInterface *_mesh);
	~VoxelMesh();

	void configureExtras(Shader* _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights) override;
	void pushPoint(GLint _vertex);
};