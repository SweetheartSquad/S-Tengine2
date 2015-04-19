#pragma once

#include <Box2DMeshEntity.h>

class Box2DWorld;
class Material;

class LD32_Donut : public Box2DMeshEntity{
public:
	static MeshInterface * donutTopMesh;
	static MeshInterface * donutBotMesh;
	static Material * donutMat;
	
	MeshEntity * bot;

	LD32_Donut(Box2DWorld * _world);
};