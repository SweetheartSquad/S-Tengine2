#include "Voxel.h"

unsigned long int Voxel::count = 0;
float Voxel::resolution = 5;
uint32_t Voxel::nextColor = 0xFFFFFF;
std::map<uint32_t, Voxel*> Voxel::voxelMap;

void Voxel::init(Vec3f _pos, bool _corrected){
	pos = _pos;
	if(!_corrected){
		pos.x -= fmod(pos.x, resolution);
		pos.y -= fmod(pos.y, resolution);
		pos.z -= fmod(pos.z, resolution);
	}
	
	voxelMap.insert(VoxelPair(nextColor, this));
	color = Color::hex(nextColor);
	nextColor -= 0x000001;
	id = count;
	count += 1;
}
Voxel::Voxel(float _x, float _y, float _z, bool _corrected = true){
	init(Vec3f(_x, _y, _z), _corrected);
	
}Voxel::Voxel(Vec3f _pos, bool _corrected = true){
	init(_pos, _corrected);
}
Voxel::~Voxel(){
	count -= 1;
}
