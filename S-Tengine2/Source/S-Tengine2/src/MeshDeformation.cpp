#pragma once

#include <MeshDeformation.h>

void MeshDeformation::flare(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing){
	sweet::Box deformerBoundingBox = _mesh->calcBoundingBox();
	
	for(auto & v : _mesh->vertices){
		//normalize vertex position by the bounding box of the mesh
		glm::vec3 vert(
			(v.x) / deformerBoundingBox.width,
			(v.y - deformerBoundingBox.y - _lowerBound) / deformerBoundingBox.height,
			(v.z) / deformerBoundingBox.depth
		);

		//flare deformation matrix
		glm::mat4 flareMatrix =  glm::mat4 (Easing::call(_easing, vert.y, 0.5f+_lowerVal, 0.5f+_upperVal,1.f), 0.f, 0.f, 0.f,
											0.f, 1.f, 0.f, 0.f,
											0.f, 0.f, Easing::call(_easing, vert.y, 0.5f+_lowerVal, 0.5f+_upperVal, 1.f), 0.f,
											0.f, 0.f, 0.f, 1.f);

		//multiply matrix by normalize vertex position vector
		glm::vec4 newVertVector4 = flareMatrix * glm::vec4(vert, 1.0f);

		//change vertex positions, scale them up by bounding box dimensions
		v.x = newVertVector4.x * deformerBoundingBox.width;
		v.y = newVertVector4.y * deformerBoundingBox.height + deformerBoundingBox.y + _lowerBound;
		v.z = newVertVector4.z * deformerBoundingBox.depth;
	}
}
void MeshDeformation::twist(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing){
	sweet::Box deformerBoundingBox = _mesh->calcBoundingBox();
	
	for(auto & v : _mesh->vertices){
		//normalize vertex position by the bounding box of the mesh
		glm::vec3 vert(
			(v.x) / deformerBoundingBox.width,
			(v.y - deformerBoundingBox.y - _lowerBound) / deformerBoundingBox.height,
			(v.z) / deformerBoundingBox.depth
		);

		//twist deformer matrix
		glm::mat4 twistMatrix = glm::mat4 (glm::cos(0.25f*vert.y), 0.f, glm::sin(0.25f*vert.y), 0.f,
											0.f, 1.f, 0.f, 0.f,
											-1.0f*(glm::sin(0.25f*vert.y)), 0.f, glm::cos(0.25f*vert.y), 0.f,
											0.f, 0.f, 0.f, 1.f);

		glm::vec4 newVertVector4 = twistMatrix * glm::vec4(vert, 1.0f);

		//change vertex positions, scale them up by bounding box dimensions
		v.x = newVertVector4.x * deformerBoundingBox.width;
		v.y = newVertVector4.y * deformerBoundingBox.height + deformerBoundingBox.y + _lowerBound;
		v.z = newVertVector4.z * deformerBoundingBox.depth;
	}
}
void MeshDeformation::bend(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing){
	sweet::Box deformerBoundingBox = _mesh->calcBoundingBox();
	
	for(auto & v : _mesh->vertices){
		//normalize vertex position by the bounding box of the mesh
		glm::vec3 vert(
			(v.x) / deformerBoundingBox.width,
			(v.y - deformerBoundingBox.y - _lowerBound) / deformerBoundingBox.height,
			(v.z) / deformerBoundingBox.depth
		);
		//bend deformer matrix
		glm::mat4 bendMatrix = glm::mat4 (glm::cos(0.25f*vert.y), glm::sin(0.25f*vert.y), 0.f, 0.f,
											Easing::call(_easing, vert.y, 0.5f, 0.1f, 1.f), Easing::call(_easing, vert.y, 0.5f, 0.1f, 1.f), 0.f, 0.f,
											0.f, 0.f, 1.f, 0.f,
											0.f, 0.f, 0.f, 1.f);
		//multiply matrix by normalize vertex position vector
		glm::vec4 newVertVector4 = bendMatrix * glm::vec4(vert, 1.0f);

		//change vertex positions, scale them up by bounding box dimensions
		v.x = newVertVector4.x * deformerBoundingBox.width;
		v.y = newVertVector4.y * deformerBoundingBox.height + deformerBoundingBox.y + _lowerBound;
		v.z = newVertVector4.z * deformerBoundingBox.depth;
	}
}