#pragma once

#include <vector>

#include "Command.h"

class Voxel;
class NodeParent;

class CMD_DeleteVoxel : public Command{
public:
	CMD_DeleteVoxel(Voxel * _v);
	~CMD_DeleteVoxel(void);

	bool execute();
	bool unexecute();

private:
	// Voxel for deletion
	Voxel * voxel;
	// Voxel for deletion's parent
	NodeParent * parent;
	// Location in parent's list of children at which voxel exists
	unsigned long int index;
};