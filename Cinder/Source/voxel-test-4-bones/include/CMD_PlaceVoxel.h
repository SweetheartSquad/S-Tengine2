#pragma once


#include <cinder/Vector.h>

#include "Command.h"

class Voxel;

class CMD_PlaceVoxel : public Command{
public:
	CMD_PlaceVoxel(ci::Vec3f _v);
	~CMD_PlaceVoxel(void);

	void execute();
	void unexecute();
private:
	ci::Vec3f v;
	Voxel * voxel;

	bool executed;
};

