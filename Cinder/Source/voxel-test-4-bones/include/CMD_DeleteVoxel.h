#pragma once

#include <vector>

#include "Command.h"

class Node;
class NodeHierarchical;
class Voxel;

class CMD_DeleteVoxel : public Command{
public:
	CMD_DeleteVoxel(Voxel * _v);
	~CMD_DeleteVoxel(void);

	void execute();
	void unexecute();

private:
	// Voxel for deletion
	Voxel * voxel;
	// Location in parent's list of children at which voxel exists
	unsigned long int index;

	// If the command has been executed
	bool executed;
};