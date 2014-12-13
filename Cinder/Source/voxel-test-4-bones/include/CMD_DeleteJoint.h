#pragma once

#include <vector>
#include "Command.h"

class Node;
class NodeHierarchical;
class NodeChild;
class Joint;

class CMD_DeleteJoint : public Command{
public:
	CMD_DeleteJoint(Joint * _jointForDeletion);
	~CMD_DeleteJoint(void);

	bool execute();
	bool unexecute();

private:
	// The joints which this command is in reference to
	Joint * jointForDeletion;
	
	// Location in jointForDeletion's parent's list of children at which jointForDeletion exists
	unsigned long int index;

	// If jointForDeletion had children, a copy of them are stored here
	std::vector<NodeChild *> children;
};