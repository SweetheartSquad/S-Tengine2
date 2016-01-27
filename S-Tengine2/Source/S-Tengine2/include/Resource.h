#pragma once

#include <node\Node.h>
#include <string>
#include <MeshInterface.h>

class VoxelJoint;
class VoxelMesh;
class TriMesh;

class Resource abstract : public Node{
public:
	/**
	* Used to load a tri mesh from an obj file. This loader
	* supports reading vertex position, normals and uvs
	* 
	* This loader is sort of slow at the moment. Hopefully
	* it will get better with further optimization
	*
	* @param _objSrc The path to the obj file
	* @returns The loaded TriMesh
	*/
	static std::vector<TriMesh *> loadMeshFromObj(std::string _objSrc, bool _autorelease);

	/**
	* Used to load a VoxelJoint from a sweet model JSON file
	* This will load all voxel points and animations 
	* withthe proper hirearchy
	* 
	* @param The src path to the json file
	* @returns The loaded VoxelJoint
	*/
	static VoxelJoint * loadVoxelModel(std::string _jsonSrc); 
};