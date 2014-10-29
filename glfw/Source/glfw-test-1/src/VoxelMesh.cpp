#include "VoxelMesh.h"

VoxelMesh::VoxelMesh(GLenum _drawMode):
	resolution(0.15),
	MeshInterface(GL_POINTS, _drawMode)
{
}

VoxelMesh::~VoxelMesh(){
}

void VoxelMesh::configureExtras(Shader* _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	glm::mat4 vp = _projectionMatrix * _viewMatrix;
	glm::mat4 m = vox::currentModelMatrix;
	GLuint vpUniformLocation = glGetUniformLocation(_shader->getProgramId(), "VP");
	GLuint mUniformLocation = glGetUniformLocation(_shader->getProgramId(), "M");
	GLuint resolutionUniformLocation = glGetUniformLocation(_shader->getProgramId(), "resolution");
	glUniformMatrix4fv(vpUniformLocation, 1, GL_FALSE, &vp[0][0]);
	glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, &m[0][0]);
	glUniform1f(resolutionUniformLocation, resolution);
}

void VoxelMesh::pushPoint(GLint _vertex){
	indices.push_back(_vertex);
}
