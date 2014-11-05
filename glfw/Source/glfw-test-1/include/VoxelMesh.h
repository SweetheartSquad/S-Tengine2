#pragma once 

#include "MeshInterface.h"

class VoxelMesh : public MeshInterface{

public:

	float resolution;

	explicit VoxelMesh(GLenum _drawMode);
	explicit VoxelMesh(MeshInterface *_mesh);
	~VoxelMesh();

	void configureExtras(Shader * _shader, MatrixStack * _matrixStack, std::vector<Light*> _lights) override;
	void pushPoint(GLint _vertex);
};