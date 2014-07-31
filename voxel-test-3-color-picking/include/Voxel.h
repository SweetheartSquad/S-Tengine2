#pragma once

#include "cinder\Vector.h"
#include "cinder\Color.h"

#include <map>

using namespace ci;

class Voxel{
public:
	static uint32_t nextColor;
	static unsigned long int count;
	static float resolution;
	static std::map<uint32_t, Voxel *> voxelMap;

	unsigned long int id;

	Vec3f pos;
	Color color;
	
	Voxel(float _x, float _y, float _z, bool _corrected);
	Voxel(Vec3f _pos, bool _corrected);
	~Voxel();
private:
	void init(Vec3f _pos, bool _corrected);

	friend std::ostream& operator<<( std::ostream& lhs, const Voxel & rhs ){
		lhs << "Voxel#" << rhs.id << std::endl 
			<< "\t[pos:\t" << rhs.pos << "]" << std::endl 
			<< "\t[color:\t" << rhs.color << "]" << std::endl;
		return lhs;
	}
};
typedef std::pair<uint32_t, Voxel*> VoxelPair;