#pragma once


#include <cinder/Vector.h>

#include "Command.h"

class Voxel;
class Joint;

class CMD_PlaceVoxel : public Command{
public:
	CMD_PlaceVoxel(ci::Vec3f _v, Joint * _parent);
	~CMD_PlaceVoxel(void);

	void execute();
	void unexecute();
private:
	ci::Vec3f v;
	Voxel * voxel;
	Joint * parent;
};

